/*
   MIPS 64 Pipeline Simulator, a la Hennesy & Patterson

   Stages are executed in reverse order - WB/MEM/EX/ID/IF
   In effect instructions are "dragged" through the pipeline 
   from the back. 
  
   However this causes problems that need to be addressed, as each stage should 
   really be executed simultaneously, taking care to do appropriate things 
   on both the Leading and Trailing clock edges.

   Forwarding is handled by maintaining two sets of registers
   the Read Registers and the Write Registers. Register status
   is also recorded, indicating where the latest register value is available,
   from, i.e. from the register itself, or forwarded from one of the various 
   pipeline registers. If a register is not available - from anywhere - we 
   get a stall.

   If forwarding is disabled, the instruction is stalled in ID. 

   If forwarding is enabled the instruction is allowed to progress until
   stalled by the unavailibility of a register.
 
   At each pipeline stage Register values are read from the Read register, 
   (indicating its status after the last clock tick) and updated to the Write 
   register. However..
  
   1. As WB takes place on the leading edge, it updates both Read and Write 
      Registers
   2. A forwarded value must be nullified once a newer value becomes 
      available, so if EX is writing to a register, both its Read and Write
      Status are updated so that earlier stages are immediately notified.
  
   At the end of a clock-tick the write registers are copied into the Read 
   registers.
  
   Note that only the most recently available value for a register is 
   stored. So a register display should only be updated if a register
   has the availability status FROM_REGISTER.

   Copyright (c) Mike Scott 2003
*/

#include "stdafx.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

/* get instruction type */

#define MMIO 0x10000

static int get_type(WORD32 instruct)
{
    int type,fmt,function,opcode,op3;
    type=0;
    opcode=(instruct >> 26);
    if (opcode==I_SPECIAL) 
    {
        function=(instruct&0x3F);
        type=REG3;
		if  (function==R_DMULU || function==R_DMUL || function==R_DDIV || function==R_DDIVU)
			type=REG3X;
        if (function==R_DSRL || function==R_DSRA || function==R_DSLL)
            type=REG2S;
        if (function==R_JR || function==R_JALR)
            type=JREG;
        if (function==R_NOP) 
			type=NOP;
        return type;
    }

    if (opcode==I_COP1)
    {
        fmt=(instruct>>21)&0x1F;
        function=(instruct&0x3F);
        if (fmt==I_DOUBLE)
        {
            type=REG3F;
            if (function==F_CVT_L_D || function==F_CVT_D_L || function==F_MOV_D)
				type=REG2F;
			if (function==F_C_LT_D || function==F_C_LE_D || function==F_C_EQ_D)
				type=REG2C;
        }
        if (fmt==I_MTC1) type=REGID;
		if (fmt==I_MFC1) type=REGDI;
		if (fmt==I_BC)   type=BC;
        return type;
    }

    type=REG2I;
    if (opcode==I_J || opcode==I_JAL) type=JUMP;
    if (opcode==I_LUI) type=REG1I;
    op3=opcode>>3;
    if (op3==0x4 || opcode==I_LD) type=LOAD;
    if (op3==0x5 || opcode==I_SD) type=STORE;
    if (opcode==I_L_D) type=FLOAD;
    if (opcode==I_S_D) type=FSTORE;
    if (opcode==I_BEQ || opcode==I_BNE) type=BRANCH;
    if (opcode==I_BEQZ || opcode==I_BNEZ) type=JREGN;
    if (opcode==I_HALT) type=HALT;

    return type;
}

static int parse(WORD32 instruct,instruction *ins)
{ // decode the instruction
    SIGNED32 extend;
    int type,r1,r2,r3,r4;

    type=get_type(instruct);

    ins->opcode=(instruct >> 26);
    ins->type=type;
    ins->function=(instruct&0x3F);

	r1=(instruct >> 16)&0x1F;
	r2=(instruct >> 21)&0x1F;
	r3=(instruct >> 11)&0x1F;
	r4=(instruct >> 6)&0x1F;

// identify the target register of this instruction (if any)
	
	ins->target=-1;
	ins->src1=-1;   // source register 1
	ins->src2=-1;   // source register 2
	ins->tf=(instruct >> 16)&0x01;

	extend=(instruct&0xFFFF);                /* sign extended */
    ins->Imm=((extend<<16)>>16);

	switch (type)
	{
	case REG3:
	case REG3X:
		ins->rt=ins->src1=r1;
		ins->rs=ins->src2=r2;
		ins->rd=r3;
		ins->target=r3;
		break;
	
	case REG2I:
		ins->rt=r1;
		ins->rs=ins->src2=r2;
		ins->rd=-1;
		ins->target=r1;
		break;

	case REG1I:
		ins->rt=r1;
		ins->rs=-1;
		ins->rd=-1;
		ins->target=r1;
		break;

	case LOAD:
		ins->rt=r1;
		ins->rs=ins->src2=r2;
		ins->rd=-1;
		ins->target=r1;
		break;

	case STORE:
		ins->rt=ins->src1=r1;
		ins->rs=ins->src2=r2;
		ins->rd=-1;
		break;

	case FSTORE:
		ins->rs=ins->src1=r2;
	    ins->rt=ins->src2=r1+32;
		ins->rd=-1;   // no rd
		break;

    case FLOAD:
		ins->rs=ins->src2=r2;
		ins->rt=r1+32;
		ins->rd=-1;  // no rd
		ins->target=ins->rt;
		break;
	
	case REG3F:
        ins->rs=ins->src1=r3+32;
        ins->rd=r4+32;
		ins->rt=ins->src2=r1+32;
		ins->target=ins->rd;
		break;

    case REG2F:
        ins->rs=ins->src1=r3+32;
		ins->rt=-1;    // no rt
        ins->rd=r4+32;
		ins->target=ins->rd;
		break;

	case REG2C:
        ins->rs=ins->src1=r3+32;
		ins->rt=ins->src2=r1+32;
		ins->rd=-1;   // no rd
		break;

	case REGID:
		ins->rs=-1;  // no rs
		ins->rt=ins->src1=r1;
		ins->rd=r3+32;
		ins->target=ins->rd;
		break;

	case REGDI:
		ins->rs=-1; // no rs
		ins->rt=r1;
		ins->rd=ins->src1=r3+32;
		ins->target=ins->rt;
		break;

	case JUMP:
		ins->rt=-1;
		ins->rs=-1;
		ins->rd=-1;
		extend=(instruct&0x3FFFFFF);
        ins->Imm=((extend<<6)>>6);
		if (ins->opcode==I_JAL) ins->target=31;
		break;
	
	case JREG: 
		ins->rt=ins->src1=r1;
	    ins->rs=-1;
		ins->rd=-1;
		if (ins->opcode==I_SPECIAL && ins->function==R_JALR) ins->target=31;
		break;
	
    case REG2S:
		ins->rt=-1;
	    ins->rs=ins->src2=r2;
		ins->rd=r3;
		ins->target=r3;
        ins->Imm=(instruct >> 6 )&0x1F;
		break;

	case JREGN:
		ins->rt=ins->src1=r1;
		ins->rs=-1;
		ins->rd=-1;
		break;

	case BRANCH:
		ins->rt=ins->src1=r1;
		ins->rs=ins->src2=r2;
		ins->rd=-1;
		break;

	case HALT:
	case NOP:
	case BC:
	default:
		ins->rt=-1;
		ins->rs=-1;
		ins->rd=-1;
		break;
	}
    return type;
}

static BOOL available(processor *cpu,int r)
{
    if (r==0) return TRUE;
    if (cpu->rreg[r].source<=NOT_AVAILABLE) return FALSE;
    return TRUE;
}

static void unavail(processor *cpu,int type,int r)
{
	if (type==READ || type == BOTH)
	{
		if (cpu->rreg[r].source>NOT_AVAILABLE) cpu->rreg[r].source=NOT_AVAILABLE;
		else 
			cpu->rreg[r].source--;  // even less available!
	}
	if (type==WRITE || type==BOTH)
	{
		if (cpu->wreg[r].source>NOT_AVAILABLE) cpu->wreg[r].source=NOT_AVAILABLE;
		else 
			cpu->wreg[r].source--;
	} 
}

static void make_available(processor *cpu,int r,WORD64 lmd)
{
	if (cpu->rreg[r].source<NOT_AVAILABLE)
		cpu->rreg[r].source++;
	else
	{
		cpu->rreg[r].source=FROM_REGISTER;
		cpu->rreg[r].val=lmd;
	}

	if (cpu->wreg[r].source<NOT_AVAILABLE)
		cpu->wreg[r].source++;
	else
	{
		cpu->wreg[r].source=FROM_REGISTER;
		cpu->wreg[r].val=lmd;
	}

}

/*
   check for WAW condition
   Check if the destination register r is going to be written 
   by one of the FP units.... it may be necessary to stall
*/

static BOOL waw(pipeline *pipe,int type,int function,int r)
{
    int i;
    switch (type)
    {
// any instruction which writes to a register...
	case REGID:
	case REGDI:
	case LOAD:
	case FLOAD:
    case FSTORE:
	case STORE:
    case REG2F:
	case REG3:
	case REG1I:
	case REG2I:
	case REG2S:
       if (pipe->div.active && pipe->div.cycles<pipe->DIV_LATENCY && pipe->div.ins.rd==r) return TRUE;
       for (i=1;i<pipe->MUL_LATENCY;i++) 
            if (pipe->m[i].active && pipe->m[i].ins.rd==r) return TRUE;
       for (i=1;i<pipe->ADD_LATENCY;i++) 
            if (pipe->a[i].active && pipe->a[i].ins.rd==r) return TRUE;
		break;
 
    case REG3F:
	case REG3X:
        switch (function)
        {
        case F_DIV_D:
		case R_DDIV:
		case R_DDIVU:
            break;        /* a divide isn't going to pass out anything... */
        case F_MUL_D:
		case R_DMUL:
		case R_DMULU:
            if (pipe->div.active && pipe->div.cycles<pipe->DIV_LATENCY && pipe->div.ins.rd==r) return TRUE;
            for (i=1;i<pipe->ADD_LATENCY;i++) 
                if (pipe->a[i].active && pipe->a[i].ins.rd==r) return TRUE;
            break;
        case F_ADD_D:
        case F_SUB_D:
            if (pipe->div.active && pipe->div.cycles<pipe->DIV_LATENCY && pipe->div.ins.rd==r) return TRUE;
            for (i=1;i<pipe->MUL_LATENCY;i++) 
                if (pipe->m[i].active && pipe->m[i].ins.rd==r) return TRUE;
            break;
        default: 
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

static int IF(pipeline *pipe,processor *cpu,BOOL delay_slot,BOOL branch_target_buffer)
{
    int type,status;
    instruction ins;
    WORD32 codeword;

/* instruction fetch */

    if (pipe->if_id.active) return STALLED;
/*
    if (cpu->cstat!=NULL)
        if (cpu->cstat[cpu->PC]) 
        {   
     //       cpu->cstat[cpu->PC]=0; // Clear Break-point 
            return BREAK;          // Break 

        }
*/
    if (pipe->active)
    {
        pipe->if_id.IR=cpu->PC;
        codeword=*(WORD32 *)&cpu->code[cpu->PC];
        type=parse(codeword,&ins);
    }
    else
    { /* if pipeline is inactive, deactivate the pipe */
        pipe->if_id.active=FALSE;
        return OK;
    }
    
    pipe->if_id.ins=ins;


/* Instruction just fetched may need to be nullified */

	pipe->if_id.active=TRUE;
	if (!delay_slot && pipe->branch) 
	{
        pipe->if_id.active=FALSE;
		pipe->if_id.ins.type=NOP;
	}
 
	if (pipe->if_id.ins.type==HALT) pipe->active=FALSE;

/* check for branches */ 
    
	pipe->if_id.NPC=cpu->PC+4;
	pipe->if_id.predicted=FALSE;

	if (!pipe->branch && branch_target_buffer && (cpu->cstat[cpu->PC]&2))
	{ // predict branch taken
		pipe->if_id.NPC = cpu->PC+4+4*ins.Imm;
		pipe->if_id.predicted=TRUE;

	}
	else
	{
		if (pipe->branch && pipe->active) pipe->if_id.NPC=pipe->destination;
	}
	status=OK;
	if (pipe->if_id.NPC>=cpu->codesize)
	{
		status=NO_SUCH_CODE_MEMORY;
		pipe->active=FALSE;
//		pipe->halting=TRUE;
	}
    else cpu->PC=pipe->if_id.NPC;

    pipe->branch=FALSE;
 
    return status;
}

static BOOL already_target(pipeline *pipe,int reg)
{
	if (pipe->div.active  && pipe->div.cycles==pipe->DIV_LATENCY  && pipe->div.ins.rd==reg)
		return TRUE;
	if (pipe->m[0].active && pipe->m[0].ins.rd==reg)
		return TRUE;
	if (pipe->a[0].active && pipe->a[0].ins.rd==reg)
		return TRUE;

// check if there is an integer instruction in EX targetting reg

	if (pipe->integer.active && pipe->integer.ins.target==reg)
		return TRUE;

	return FALSE;
}

static BOOL already_waitedfor(pipeline *pipe,int reg)
{
	if (pipe->m[0].active)
	{
		if (pipe->m[0].ins.rs==reg || pipe->m[0].ins.rt==reg) return TRUE;
	}
	if (pipe->a[0].active)
	{
		if (pipe->a[0].ins.rs==reg || pipe->a[0].ins.rt==reg) return TRUE;
	}
	if (pipe->div.active && pipe->div.cycles==pipe->DIV_LATENCY)
	{
		if (pipe->div.ins.rs==reg || pipe->div.ins.rt==reg) return TRUE;
	}
/* V1.53 */
	if (pipe->integer.active)
	{
		if (pipe->integer.ins.src1==reg || pipe->integer.ins.src2==reg) return TRUE;
	}
	return FALSE;
}

static int ID(pipeline *pipe,processor *cpu,BOOL forwarding,BOOL branch_target_buffer,int *rawreg)
{
    int status,type,branch_status;
    instruction ins;
	BOOL predictable;
    WORD64 A,B;
	BOOL branch_complete;

    if (!pipe->if_id.active) return EMPTY;      /* nothing to do   */

    ins=pipe->if_id.ins;
    type=ins.type;


	if (pipe->integer.active)
	{

		if (type!=REG3F && type!=REG3X)  return STALLED; /* exit is blocked */
/* get A and B registers as required */
	//	else
	//	{
/* Stall in ID on WAR hazard V1.53 Fix */
	//		if (forwarding && (pipe->integer.rA==ins.rd || pipe->integer.rB==ins.rd))
	//		{ 
	//			*rawreg=ins.rd;
	//			return WAR;
	//		}
	//	}
	}

    switch (type)
    {
    case LOAD:
    case FLOAD:
	case REG2I:
    case REG2S:
	case REG2F:
		if (!forwarding)
		{ // Stall it in ID - must wait for reg to be Written Back 
			if (!available(cpu,ins.rs)) {*rawreg=ins.rs; return RAW;}
		}
        pipe->integer.rA=ins.rs;
		pipe->integer.rB=-1;
        break;

	case REGID:
		if (!forwarding)
		{ // Stall it in ID - must wait for reg to be Written Back 
			if (!available(cpu,ins.rt)) {*rawreg=ins.rt; return RAW;}
		}
        pipe->integer.rA=ins.rt;
		pipe->integer.rB=-1;
		break;

	case REGDI:
		if (!forwarding)
		{ // Stall it in ID - must wait for reg to be Written Back 
			if (!available(cpu,ins.rd)) {*rawreg=ins.rd; return RAW;}
		}
        pipe->integer.rA=ins.rd;
		pipe->integer.rB=-1;
		break;


    case STORE:  
    case REG3:
    case FSTORE:
	case REG2C:
		if (!forwarding)
		{
			if (!available(cpu,ins.rt)) {*rawreg=ins.rt; return RAW;}
			if (!available(cpu,ins.rs)) {*rawreg=ins.rs; return RAW;}
		}
        pipe->integer.rA=ins.rs;
        pipe->integer.rB=ins.rt;
        break;

	case REG3F:
	case REG3X:
		if (!forwarding)
		{
			if (!available(cpu,ins.rt)) {*rawreg=ins.rt; return RAW;}
			if (!available(cpu,ins.rs)) {*rawreg=ins.rs; return RAW;}
		} 
		break;
    case JREG:
    case JREGN:
        if (!available(cpu,ins.rt)) {*rawreg=ins.rt; return RAW;}
        B=cpu->rreg[ins.rt].val;
        break;
    case BRANCH:
        if (!available(cpu,ins.rs)) {*rawreg=ins.rs; return RAW;}
		if (!available(cpu,ins.rt)) {*rawreg=ins.rt; return RAW;}
        A=cpu->rreg[ins.rs].val;
        B=cpu->rreg[ins.rt].val;
        break;
    default: 
        break;
    }

 /* jumps and branches (and FPs) */
	branch_complete=FALSE;
	branch_status=NOT_A_BRANCH;
	predictable=FALSE;
 /*
	Lots of good reasons for holding an instruction back in ID
	For example output register is waiting to be read by instruction in front - 
	must not overtake it!
	Any of the registers are waiting for pipeline completions!
 */
	
	switch (type)
    {
	case REG2F:
	case REG2S:
		if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
		if (already_target(pipe,ins.rd)) {*rawreg=ins.rd; return WAW;}
		if (already_waitedfor(pipe,ins.rd)) {*rawreg=ins.rd; return WAR;} // new
		break;
	case REG2C:
	case STORE:
	case FSTORE:
		if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
		break;
	case REG2I:
	case LOAD:
	case FLOAD:
		if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
		if (already_waitedfor(pipe,ins.rt)) {*rawreg=ins.rt; return WAR;}
		break;
	case REG1I:
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
		if (already_waitedfor(pipe,ins.rt)) {*rawreg=ins.rt; return WAR;}
		break;
	case REG3:
		if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
		if (already_target(pipe,ins.rd)) {*rawreg=ins.rd; return WAW;}
		if (already_waitedfor(pipe,ins.rd)) {*rawreg=ins.rd; return WAR;}
		break;
	case REGDI:
	case REGID:
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
		if (already_target(pipe,ins.rd)) {*rawreg=ins.rd; return WAW;}
		if (already_waitedfor(pipe,ins.rd)) {*rawreg=ins.rd; return WAR;} 
		break;
	case REG3F:
	case REG3X:
        switch (ins.function)
        {
            case F_ADD_D:
            case F_SUB_D:
                if (pipe->a[0].active) return STALLED;
				if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
				if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
				if (already_target(pipe,ins.rd)) {*rawreg=ins.rd; return WAW;}
				if (already_waitedfor(pipe,ins.rd)) {*rawreg=ins.rd; return WAR;}
				pipe->a[0].rA=ins.rs;
				pipe->a[0].rB=ins.rt;
		        pipe->a[0].active=TRUE;
				pipe->a[0].NPC=pipe->if_id.NPC;
				pipe->a[0].IR =pipe->if_id.IR;
				pipe->a[0].ins=ins;
                break;
            case F_MUL_D:
			case R_DMUL:
			case R_DMULU:
//char txt[80];
//sprintf(txt,"need ins.rs= %d and ins.rt= %d",ins.rs,ins.rt);
//AfxMessageBox(txt);
                if (pipe->m[0].active) return STALLED;
				if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
				if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
				if (already_target(pipe,ins.rd)) {*rawreg=ins.rd; return WAW;}
				if (already_waitedfor(pipe,ins.rd)) {*rawreg=ins.rd; return WAR;}
				pipe->m[0].rA=ins.rs;
				pipe->m[0].rB=ins.rt;
		        pipe->m[0].active=TRUE;
				pipe->m[0].NPC=pipe->if_id.NPC;
				pipe->m[0].IR =pipe->if_id.IR;
				pipe->m[0].ins=ins;
			//	unavail(cpu,BOTH,ins.rd);
                break;
            case F_DIV_D:
			case R_DDIV:
			case R_DDIVU:
                if (pipe->div.active) return STRUCTURAL;
				if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
				if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
				if (already_target(pipe,ins.rd)) {*rawreg=ins.rd; return WAW;}
				if (already_waitedfor(pipe,ins.rd)) {*rawreg=ins.rd; return WAR;}
				pipe->div.rA=ins.rs;
				pipe->div.rB=ins.rt;
		        pipe->div.active=TRUE;
				pipe->div.NPC=pipe->if_id.NPC;
				pipe->div.IR =pipe->if_id.IR;
				pipe->div.ins=ins;
				pipe->div.cycles=pipe->DIV_LATENCY;
                break;
            default:
                break;
        }
		pipe->if_id.active=FALSE;
		return OK;
    case BRANCH:
		if (already_target(pipe,ins.rs)) {*rawreg=ins.rs; return RAW;}
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
	
		branch_complete=TRUE;
		branch_status=BRANCH_NOT_TAKEN;
		predictable=TRUE;
        if (ins.opcode==I_BEQ && (A==B))
        {
			branch_status=BRANCH_TAKEN;
            pipe->branch=TRUE;
            pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
        }
        if (ins.opcode==I_BNE && (A!=B))
        { 
			branch_status=BRANCH_TAKEN;
            pipe->branch=TRUE;
            pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
        }
		pipe->if_id.active=FALSE;
        break;

	case BC:
		branch_complete=TRUE;
		branch_status=BRANCH_NOT_TAKEN;
		predictable=TRUE;
		if (ins.tf && cpu->fp_cc)
		{
			branch_status=BRANCH_TAKEN;
			pipe->branch=TRUE;
			pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
		}
		if (!ins.tf && !cpu->fp_cc)
		{
			branch_status=BRANCH_TAKEN;
			pipe->branch=TRUE;
			pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
		}
		pipe->if_id.active=FALSE;
		break;

    case JREGN:
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
		
		branch_complete=TRUE;
		branch_status=BRANCH_NOT_TAKEN;
		predictable=TRUE;
        if (B==0 && ins.opcode==I_BEQZ) 
        {
			branch_status=BRANCH_TAKEN;
            pipe->branch=TRUE;
            pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
        }
        if (B!=0 && ins.opcode==I_BNEZ) 
        {     
			branch_status=BRANCH_TAKEN;
            pipe->branch=TRUE;
            pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
        }
		pipe->if_id.active=FALSE;
        break;
    case JUMP:
		predictable=TRUE;
		branch_status=BRANCH_TAKEN;
        if (ins.opcode==I_J)
        {
			branch_complete=TRUE;		
            pipe->branch=TRUE;
            pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
			pipe->if_id.active=FALSE;
        }
        if (ins.opcode==I_JAL)
        {
            pipe->branch=TRUE;
            pipe->destination=pipe->if_id.NPC+4*pipe->if_id.ins.Imm;
       //     if (forwarding)
       //     {
       //         cpu->wreg[31].val=pipe->if_id.NPC;
       //         cpu->wreg[31].source=FROM_ID;
       //     }
       //     else
	//			unavail(cpu,WRITE,31);
        }
        break;
    case JREG:
		if (already_target(pipe,ins.rt)) {*rawreg=ins.rt; return RAW;}
	
		branch_status=BRANCH_TAKEN;
        if (ins.opcode==I_SPECIAL && ins.function==R_JR)
        {
			branch_complete=TRUE;
            pipe->branch=TRUE;
            pipe->destination=(WORD32)B;
			pipe->if_id.active=FALSE;
        }
    //    break;
        if (ins.opcode==I_SPECIAL && ins.function==R_JALR)
        {
            pipe->branch=TRUE;
            pipe->destination=(WORD32)B;
        //    if (forwarding)
        //    {
        //        cpu->wreg[31].val=pipe->if_id.NPC;
        //        cpu->wreg[31].source=FROM_ID;
        //    }
        //    else
		//		unavail(cpu,WRITE,31);
        }
        break;
    default:
        break;
    }

// indicate branch taken from this address
// but only if branch target buffer is enabled.

	status=OK;

	if (branch_target_buffer && predictable)
	{
		if (branch_status==BRANCH_TAKEN)
		{
			status=BRANCH_TAKEN_STALL;
			if ((cpu->cstat[pipe->if_id.IR]&2)==0) 
			{ // throw in an extra stall...
				cpu->cstat[pipe->if_id.IR]|=2;
				pipe->if_id.active=TRUE;
				return status;
			}
			if (pipe->if_id.predicted) 
			{
				pipe->branch=FALSE;
				status=OK;
			}
		}
		if (branch_status==BRANCH_NOT_TAKEN)
		{
			if (pipe->if_id.predicted)
			{ // its was predicted, but it didn't happen!
				status=BRANCH_MISPREDICTED_STALL;
				if (cpu->cstat[pipe->if_id.IR]&2)
				{
					cpu->cstat[pipe->if_id.IR]&=0xfd;
					pipe->if_id.active=TRUE;
					return status;
				}
			// tell IF to re-fetch from here + 4 - costs another stall!
				pipe->branch=TRUE;
				pipe->destination=pipe->if_id.IR+4;
			
			}
		}
	}
	else
	{
		if (branch_status==BRANCH_TAKEN) status=BRANCH_TAKEN_STALL;
	}


    pipe->integer.NPC=pipe->if_id.NPC;
    pipe->integer.IR =pipe->if_id.IR;
    pipe->integer.ins=ins;
    pipe->integer.Imm=ins.Imm;

/* If branch instruction has completed - deactivate it so as 
   not to cause problems further down the pipe.               */
	
/*	if (branch_complete)	pipe->integer.active=FALSE;
	else		*/
	pipe->integer.active=TRUE;
    pipe->if_id.active=FALSE;


    return status;
}

static int EX_DIV(pipeline *pipe,processor *cpu,BOOL forwarding,int *rawreg)
{
	int rA,rB,DIVS;
    instruction ins;  
    DOUBLE64 fpA,fpB,fpR;
	BOOL status=EMPTY;

/*
   if the division unit is active, count it down by one
   if cycles = 0 and active is TRUE, then result is available
*/

	ins=pipe->div.ins;
	rA=pipe->div.rA;
	rB=pipe->div.rB;
	DIVS=pipe->DIV_LATENCY;
    if (pipe->div.active)
    { /* Have I got a result ?? */

		if (pipe->div.cycles==pipe->DIV_LATENCY)
		{ // Trying to start a new one...
			if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
			if (!available(cpu,rB)) {*rawreg=rB; return RAW;}
			if (waw(pipe,ins.type,ins.function,ins.rd)) {*rawreg=ins.rd; return WAW;} //new

			unavail(cpu,BOTH,ins.rd);
			fpA.u=cpu->rreg[rA].val;
			fpB.u=cpu->rreg[rB].val;

			status=OK;
			switch (ins.function)
			{
            case F_DIV_D:
				if (fpB.d!=0.0) fpR.d=fpA.d/fpB.d;
				else            status=DIVIDE_BY_ZERO;
				break;
			case R_DDIV:
				if (fpB.s!=0)	fpR.s=fpA.s/fpB.s;
				else            status=DIVIDE_BY_ZERO;				
				break;
			case R_DDIVU:
				if (fpB.u!=0)	fpR.u=fpA.u/fpB.u;
				else            status=DIVIDE_BY_ZERO;				

				break;
			default:
				break;
			}

			pipe->div.ALUOutput=fpR.u;
			pipe->div.cycles--;
		}
		else
			if (pipe->div.cycles>0) 
			{
				pipe->div.cycles--;
				status=OK;
			}
        if (pipe->div.cycles==0)
		{
			if (!pipe->ex_mem.active)
			{ /*  finish divide  */
			
				pipe->ex_mem.ins=ins;
				pipe->ex_mem.IR=pipe->div.IR;

				pipe->ex_mem.ALUOutput=pipe->div.ALUOutput;
				if (forwarding)
				{  
					cpu->wreg[ins.rd].val=pipe->div.ALUOutput;
					cpu->wreg[ins.rd].source=FROM_DIV;
				}
				pipe->ex_mem.active=TRUE;
				pipe->ex_mem.NPC=pipe->div.NPC;
				pipe->ex_mem.rB=pipe->div.rB;
				pipe->div.active=FALSE;
				return status;
			}
			else return STALLED;
		}
    }

	return status;
}


// Note that both the instruction at the front AND the instruction at the back
// can suffer stalls... so seperate status for each 

static void EX_MUL(pipeline *pipe,processor *cpu,BOOL forwarding,int *rawreg,int *status)
{
	int i,rA,rB,MULS;
	instruction ins;
	DOUBLE64 fpA,fpB,fpR;
    id_ex_reg idle;

	MULS=pipe->MUL_LATENCY;
	for (i=0;i<MULS;i++) status[i]=OK;
	ins=pipe->m[MULS-1].ins;     
    idle.active=FALSE;
    if (pipe->m[MULS-1].active)   // last one active
    {
        if (!pipe->ex_mem.active)
        { /* execute multiply */
  
            pipe->ex_mem.ins=ins;
            pipe->ex_mem.IR=pipe->m[MULS-1].IR;

            pipe->ex_mem.ALUOutput=pipe->m[MULS-1].ALUOutput;
            if (forwarding)
            { 
                cpu->wreg[ins.rd].val=pipe->m[MULS-1].ALUOutput;
                cpu->wreg[ins.rd].source=FROM_MUL;
            }

			pipe->m[MULS-1].active=FALSE;
            pipe->ex_mem.active=TRUE;
			pipe->ex_mem.NPC=pipe->m[MULS-1].NPC;
			pipe->ex_mem.rB=pipe->m[MULS-1].rB;
        }
		else status[MULS-1]=STALLED; 
    }
	else status[MULS-1]=EMPTY;

// shift them over if possible
    for (i=MULS-1;i>1;i--)
	{
        if (!pipe->m[i].active) 
		{
			pipe->m[i]=pipe->m[i-1];
			pipe->m[i-1].active=FALSE;
		}
		else if (pipe->m[i-1].active) status[i-1]=STALLED;
	}	

	if (!pipe->m[1].active)
	{

		if (pipe->m[0].active)
		{ // Trying to start a new one...
			rA=pipe->m[0].rA;
			rB=pipe->m[0].rB;
			ins=pipe->m[0].ins;	

			if (!available(cpu,rA)) {*rawreg=rA; status[0]=RAW; return;}
			if (!available(cpu,rB)) {*rawreg=rB; status[0]=RAW; return;}
			
			if (waw(pipe,ins.type,ins.function,ins.rd)) {*rawreg=ins.rd; status[0]=WAW; return;} // new

			unavail(cpu,BOTH,ins.rd);

            fpA.u=cpu->rreg[rA].val;
            fpB.u=cpu->rreg[rB].val;
			switch (ins.function)
			{
            case F_MUL_D:
				fpR.d=fpA.d*fpB.d;
				break;
			case R_DMUL:
				fpR.s=fpA.s*fpB.s;
				break;
			case R_DMULU:
				fpR.u=fpA.u*fpB.u;
				break;
			default:
				break;
			}
         
			pipe->m[0].ALUOutput=fpR.u;

		}
		pipe->m[1]=pipe->m[0];
		pipe->m[0]=idle;
	}
	else if (pipe->m[0].active) status[0]=STALLED;

}

static void EX_ADD(pipeline *pipe,processor *cpu,BOOL forwarding,int *rawreg,int *status)
{
	int i,rA,rB,ADDS;
	instruction ins;
	DOUBLE64 fpA,fpB,fpR;
    id_ex_reg idle;
	
	ADDS=pipe->ADD_LATENCY;
	for (i=0;i<ADDS;i++) status[i]=OK;
    ins=pipe->a[ADDS-1].ins;     
    idle.active=FALSE;
    if (pipe->a[ADDS-1].active)
    {
        if (!pipe->ex_mem.active)
        { /* execute addition */
             
            pipe->ex_mem.ins=ins;
            pipe->ex_mem.IR=pipe->a[ADDS-1].IR;

            pipe->ex_mem.ALUOutput=pipe->a[ADDS-1].ALUOutput;
            if (forwarding)
            { 
                cpu->wreg[ins.rd].val=pipe->a[ADDS-1].ALUOutput;
                cpu->wreg[ins.rd].source=FROM_ADD;
            }

			pipe->a[ADDS-1].active=FALSE;
            pipe->ex_mem.active=TRUE;
 			pipe->ex_mem.NPC=pipe->a[ADDS-1].NPC;
			pipe->ex_mem.rB=pipe->a[ADDS-1].rB;
        }
		else status[ADDS-1]=STALLED;
    }
    else status[ADDS-1]=EMPTY;

   /* shift forward by one and put NOP in the start */
    for (i=ADDS-1;i>1;i--)
	{
		if (!pipe->a[i].active)
		{
			pipe->a[i]=pipe->a[i-1];
			pipe->a[i-1].active=FALSE;
		}
		else if (pipe->a[i-1].active) status[i-1]=STALLED;
	}

	if (!pipe->a[1].active)
	{
		if (pipe->a[0].active)
		{ // Trying to start a new one...
			rA=pipe->a[0].rA;
			rB=pipe->a[0].rB;
			ins=pipe->a[0].ins;

			if (!available(cpu,rA)) {*rawreg=rA; status[0]=RAW; return;}
			if (!available(cpu,rB)) {*rawreg=rB; status[0]=RAW; return;}
			if (waw(pipe,ins.type,ins.function,ins.rd)) {*rawreg=ins.rd; status[0]=WAW; return;}

			unavail(cpu,BOTH,ins.rd);

            fpA.u=cpu->rreg[rA].val;
            fpB.u=cpu->rreg[rB].val;
			switch (ins.function)
			{
            case F_ADD_D:
				fpR.d=fpA.d+fpB.d;
				break;
            case F_SUB_D:
				 fpR.d=fpA.d-fpB.d;
			default:
				break;
			}
			pipe->a[0].ALUOutput=fpR.u;

		}
		pipe->a[1]=pipe->a[0];
		pipe->a[0]=idle;
	}
	else if (pipe->a[0].active) status[0]=STALLED;

}

static int EX_INT(pipeline *pipe,processor *cpu,BOOL forwarding,int *rawreg)
{
    int rA,rB,opcode,type,function;
	SIGNED64 rlt;
    instruction ins;
	BOOL condition=TRUE;
    int status=OK;
    
    DOUBLE64 fpA,fpB,fpR;

	if (pipe->integer.active)
	{
		if (pipe->ex_mem.active) return STALLED;
    }
	else return EMPTY;
 
    ins=pipe->integer.ins;
    type=ins.type;
    function=ins.function;
    opcode=ins.opcode;

/* All systems go. Get instruction, and pass to appropriate unit
   Mark destination register as not available for earlier stages...
*/
    rA=pipe->integer.rA;
    rB=pipe->integer.rB;

    switch (type)
    {
    case FLOAD:
        if (waw(pipe,type,function,ins.rt)) {*rawreg=ins.rt; return WAW;}
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
		unavail(cpu,READ,ins.rt);
        fpA.u=cpu->rreg[rA].val;
        pipe->ex_mem.ALUOutput=fpA.u + ins.Imm;
        break;
    case LOAD:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
		unavail(cpu,READ,ins.rt);
        fpA.u=cpu->rreg[rA].val;
        pipe->ex_mem.ALUOutput=fpA.u + ins.Imm;
        break;
    case FSTORE:
    case STORE:
        if (waw(pipe,type,function,ins.rt)) {*rawreg=ins.rt; return RAW;}  
													/* bodge */
	
   /* calculate address for memory access - same for all flavours */
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
        fpA.u=cpu->rreg[rA].val;
        pipe->ex_mem.ALUOutput=fpA.u + ins.Imm;
        break;
    case REG1I:
		if (waw(pipe,type,function,ins.rt)) {*rawreg=ins.rt; return WAW;} //new
		unavail(cpu,BOTH,ins.rt);
        switch (opcode)
        {
        case I_LUI:
            pipe->ex_mem.ALUOutput=((SIGNED64)ins.Imm<<16); /* thanks Katia */
        default:
            break;
        }
        if (forwarding && ins.rt!=0) 
        {
            cpu->wreg[ins.rt].val=pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rt].source=FROM_EX;
        }
        break;
    case REG2I:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
		if (waw(pipe,type,function,ins.rt)) {*rawreg=ins.rt; return WAW;} //new
		unavail(cpu,BOTH,ins.rt);

        fpA.u=cpu->rreg[rA].val;
        switch (opcode)
        {
        case I_DADDI:
			rlt=fpA.s + ins.Imm;
            pipe->ex_mem.ALUOutput = rlt;
			if (ins.Imm>0 && rlt<fpA.s) status=INTEGER_OVERFLOW;
			if (ins.Imm<0 && rlt>fpA.s) status=INTEGER_OVERFLOW;
            break;
        case I_DADDIU:
            pipe->ex_mem.ALUOutput = fpA.u + ins.Imm;
            break;
        case I_ANDI:
            pipe->ex_mem.ALUOutput = fpA.u & ins.Imm;
            break;
        case I_ORI:
            pipe->ex_mem.ALUOutput = fpA.u | ins.Imm;
            break;
        case I_XORI:
            pipe->ex_mem.ALUOutput = fpA.u ^ ins.Imm;
            break;
        case I_SLTI:
            if (fpA.s < ins.Imm) pipe->ex_mem.ALUOutput=1;
            else                 pipe->ex_mem.ALUOutput=0;
            break;
        case I_SLTIU:
            if (fpA.u < ins.Imm) pipe->ex_mem.ALUOutput=1;
            else                 pipe->ex_mem.ALUOutput=0;
            break;
        }
        if (forwarding && ins.rt!=0) 
        {
            cpu->wreg[ins.rt].val = pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rt].source = FROM_EX;
        }
        break;
    case REG2S:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
		if (waw(pipe,type,function,ins.rd)) {*rawreg=ins.rd; return WAW;} //new
		unavail(cpu,BOTH,ins.rd);
        fpA.u=cpu->rreg[rA].val;
        switch (function)
        {
        case R_DSLL:
            pipe->ex_mem.ALUOutput=fpA.u << ins.Imm;    
            break;
        case R_DSRL:
            pipe->ex_mem.ALUOutput=fpA.u >> ins.Imm;    
            break;
        case R_DSRA:
            pipe->ex_mem.ALUOutput=fpA.s >> ins.Imm;    
            break;
        default: 
            break; 
        }
        if (forwarding && ins.rd!=0)
        {
            cpu->wreg[ins.rd].val = pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rd].source = FROM_EX;
        }
        break;
    case JUMP:
    case JREG:
		if (ins.opcode==I_JAL || (ins.opcode==I_SPECIAL && ins.function==R_JALR))
		{
			unavail(cpu,BOTH,31);
		    if (forwarding)
            {
                cpu->wreg[31].val=pipe->integer.NPC;
                cpu->wreg[31].source=FROM_EX;
            }
         //   else
		//		unavail(cpu,WRITE,31);
		}
      //  if (forwarding && opcode==I_JAL || (opcode==I_SPECIAL && function==R_JALR))
      //      cpu->wreg[31].source = FROM_EX;
        break;
    case HALT:
        break;

    case REG3:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
		if (!available(cpu,rB)) {*rawreg=rB; return RAW;}
		if (waw(pipe,type,function,ins.rd)) {*rawreg=ins.rd; return WAW;} //new
        fpA.u=cpu->rreg[rA].val;
        fpB.u=cpu->rreg[rB].val;
	
        switch (function)
        {
        case R_AND:
            pipe->ex_mem.ALUOutput=fpA.u & fpB.u;
            break;
        case R_OR:
            pipe->ex_mem.ALUOutput=fpA.u | fpB.u;
            break;
        case R_XOR:
            pipe->ex_mem.ALUOutput=fpA.u ^ fpB.u;
            break;
        case R_SLT:
            if (fpA.s < fpB.s) pipe->ex_mem.ALUOutput=1;
            else               pipe->ex_mem.ALUOutput=0;
            break;
        case R_SLTU:
            if (fpA.u < fpB.u) pipe->ex_mem.ALUOutput=1;
            else               pipe->ex_mem.ALUOutput=0;
            break;
        case R_DADD:
			rlt=fpA.s+fpB.s;
            pipe->ex_mem.ALUOutput=rlt;
			if (rlt<fpA.s) status=INTEGER_OVERFLOW;
            break;
        case R_DADDU:
            pipe->ex_mem.ALUOutput=fpA.u + fpB.u;
            break;
        case R_DSUB:
			rlt=fpA.s - fpB.s;
            pipe->ex_mem.ALUOutput=rlt;
			if (rlt>fpA.s) status=INTEGER_OVERFLOW;
            break;
        case R_DSUBU:
            pipe->ex_mem.ALUOutput=fpA.u - fpB.u;
            break;
 
		case R_DSLLV:
            pipe->ex_mem.ALUOutput=fpA.u << (fpB.u & 0x3F);
            break;
        case R_DSRLV:
            pipe->ex_mem.ALUOutput=fpA.u >> (fpB.u & 0x3F);
            break;
        case R_DSRAV:
            pipe->ex_mem.ALUOutput=fpA.s >> (fpB.u & 0x3F);
            break;
        case R_MOVZ:
            if (fpB.u==0) pipe->ex_mem.ALUOutput=fpA.u;
			else condition=FALSE;
            break;
        case R_MOVN:
            if (fpB.u!=0) pipe->ex_mem.ALUOutput=fpA.u;
			else condition=FALSE;
            break;
        default:
            break;
        }
        if (condition) 
		{
			unavail(cpu,BOTH,ins.rd);
		}
		if (condition && forwarding && ins.rd!=0)
        {
            cpu->wreg[ins.rd].val = pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rd].source = FROM_EX;
        }
        break;
    case REGID:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
        if (waw(pipe,type,function,ins.rd)) {*rawreg=ins.rd; return WAW;}
		unavail(cpu,BOTH,ins.rd);
        fpA.u=cpu->rreg[rA].val;
        pipe->ex_mem.ALUOutput=fpA.s;
        if (forwarding && ins.rd!=0)
        {
            cpu->wreg[ins.rd].val = pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rd].source = FROM_EX;
        }
        break;
    case REGDI:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
        if (waw(pipe,type,function,ins.rt)) {*rawreg=ins.rt; return WAW;}
		unavail(cpu,BOTH,ins.rt);
        fpA.u=cpu->rreg[rA].val;
        pipe->ex_mem.ALUOutput=fpA.s;
        if (forwarding && ins.rt!=0)
        {
            cpu->wreg[ins.rt].val = pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rt].source = FROM_EX;
        }
        break;
    case JREGN:
        break;

	case REG2C:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
		if (!available(cpu,rB)) {*rawreg=rB; return RAW;}
		fpA.u=cpu->rreg[rA].val;
		fpB.u=cpu->rreg[rB].val;
		cpu->fp_cc=FALSE;
		switch (function)
		{
		case F_C_LT_D:
			if (fpA.d< fpB.d) cpu->fp_cc=TRUE;
			break;
		case F_C_LE_D:
			if (fpA.d<=fpB.d) cpu->fp_cc=TRUE;
			break;
		case F_C_EQ_D:
			if (fpA.d==fpB.d) cpu->fp_cc=TRUE;
			break;
		
		}
		break;

    case REG2F:
        if (!available(cpu,rA)) {*rawreg=rA; return RAW;}
        if (waw(pipe,type,function,ins.rd)) {*rawreg=ins.rd; return WAW;} //new

		unavail(cpu,BOTH,ins.rd);

        fpA.u=cpu->rreg[rA].val;
        switch (function)
        {
        case F_CVT_D_L:
            fpR.d = (double)fpA.s;
            pipe->ex_mem.ALUOutput=fpR.u;
            break;
		case F_CVT_L_D:
            fpR.s = (__int64)fpA.d;
            pipe->ex_mem.ALUOutput=fpR.u;
            break;
		case F_MOV_D:
			fpR.u=fpA.u;
			pipe->ex_mem.ALUOutput=fpR.u;
			break;
        default:
            break;
        }
        if (forwarding && ins.rd!=0)
        {
            cpu->wreg[ins.rd].val = pipe->ex_mem.ALUOutput;
            cpu->wreg[ins.rd].source = FROM_EX;
        }
        break;

    case NOP:
    default:
        break;
    }
    pipe->ex_mem.IR=pipe->integer.IR;
    pipe->ex_mem.ins=ins;
    pipe->ex_mem.NPC=pipe->integer.NPC;
    pipe->ex_mem.rB=pipe->integer.rB;
    pipe->ex_mem.active=TRUE;
	pipe->ex_mem.condition=condition;
    pipe->integer.active=FALSE;
    
    return status;
}

static int MEM(pipeline *pipe,processor *cpu,BOOL forwarding,int *rawreg)
{
    int i,opcode,function,status,type,rB;
    instruction ins; 
    BYTE   b;
    WORD16 h;
    WORD32 w,ptr;
    WORD64  u;
    SIGNED64 s;
	BOOL condition,mmio;

    pipe->mem_wb.active=FALSE;
    if (!pipe->ex_mem.active) return EMPTY;

	condition=pipe->ex_mem.condition;
    ins=pipe->ex_mem.ins;
    pipe->mem_wb.ins=ins;

    type=ins.type;
    status=OK;
    opcode=ins.opcode;
    function=ins.function;
    ptr=(WORD32)pipe->ex_mem.ALUOutput;

    switch (type)
    {
    case LOAD:
    case FLOAD:
		status=LOADS;
		unavail(cpu,BOTH,ins.rt);
      
		if (ptr>=cpu->datasize && (ptr<MMIO || ptr>=MMIO+16))
		{
			pipe->mem_wb.LMD=0;
			status=NO_SUCH_DATA_MEMORY;
		}
        else 
		{
			if (ptr>=MMIO && ptr<(MMIO+16)) mmio=TRUE;
			else mmio=FALSE;

			switch (opcode)
		    {
		case I_LB:
				if (mmio)
					b=*(BYTE *)(&cpu->mm[ptr-MMIO]);
				else
				{
					if (cpu->dstat[ptr]==VACANT) status=DATA_ERR;
					b=*(BYTE *)(&cpu->data[ptr]);
				}
				s=b;
				pipe->mem_wb.LMD=(s<<56)>>56;
				break;
        case I_LBU:
				if (mmio)
					b=*(BYTE *)(&cpu->mm[ptr-MMIO]);
				else
				{
					if (cpu->dstat[ptr]==VACANT) status=DATA_ERR;
					b=*(BYTE *)(&cpu->data[ptr]);
				}
				u=b;
				pipe->mem_wb.LMD=(u<<56)>>56;
				break;
        case I_LH:
				if (ptr%2!=0)
				{
					status=DATA_MISALIGNED;
					pipe->mem_wb.LMD=0;
					break;
				}
				if (mmio)
					h=*(WORD16 *)(&cpu->mm[ptr-MMIO]);
				else
				{
					for (i=0;i<2;i++) 
						if (cpu->dstat[ptr+i]==VACANT) status=DATA_ERR;
					h=*(WORD16 *)(&cpu->data[ptr]);
				}
				s=h;
				pipe->mem_wb.LMD=(s<<48)>>48;
				break;
        case I_LHU:
				if (ptr%2!=0)
				{
					status=DATA_MISALIGNED;
					pipe->mem_wb.LMD=0;
					break;
				}
				if (mmio)
					h=*(WORD16 *)(&cpu->mm[ptr-MMIO]);
				else
				{
					for (i=0;i<2;i++) 
						if (cpu->dstat[ptr+i]==VACANT) status=DATA_ERR;
					h=*(WORD16 *)(&cpu->data[ptr]);
				}
				u=h;
				pipe->mem_wb.LMD=(u<<48)>>48;
				break;
        case I_LW:
 				if (ptr%4!=0)
				{
					status=DATA_MISALIGNED;
					pipe->mem_wb.LMD=0;
					break;
				}
				if (mmio)
					w=*(WORD32 *)(&cpu->mm[ptr-MMIO]);
				else
				{
					for (i=0;i<4;i++) 
						if (cpu->dstat[ptr+i]==VACANT) status=DATA_ERR;
					w=*(WORD32 *)(&cpu->data[ptr]);
				}
				s=w;
				pipe->mem_wb.LMD=(s<<32)>>32;
				break;
        case I_LWU:
				if (ptr%4!=0)
				{
					status=DATA_MISALIGNED;
					pipe->mem_wb.LMD=0;
					break;
				}
				if (mmio)
					w=*(WORD32 *)(&cpu->mm[ptr-MMIO]);
				else
				{
					for (i=0;i<4;i++) 
						if (cpu->dstat[ptr+i]==VACANT) status=DATA_ERR;
					w=*(WORD32 *)(&cpu->data[ptr]);
				}
//AfxMessageBox("Hello");
				u=w;
				pipe->mem_wb.LMD=(u<<32)>>32;
				break;
        case I_LD:
        case I_L_D:
				if (ptr%8!=0)
				{
					status=DATA_MISALIGNED;
					pipe->mem_wb.LMD=0;
					break;
				}
				if (mmio)
					pipe->mem_wb.LMD=*(WORD64 *)(&cpu->mm[ptr-MMIO]);
				else
				{
					for (i=0;i<8;i++) 
						if (cpu->dstat[ptr+i]==VACANT) status=DATA_ERR;
					pipe->mem_wb.LMD=*(WORD64 *)(&cpu->data[ptr]);
				}
				break;
			default:
				break;
			}
		}
        if (forwarding && ins.rt!=0)
        {
            cpu->wreg[ins.rt].val=pipe->mem_wb.LMD;
            cpu->wreg[ins.rt].source=FROM_MEM;
        }
        break;
    case STORE:
    case FSTORE:
        rB=pipe->ex_mem.rB;
        if (!available(cpu,rB)) {*rawreg=rB; return RAW;}
		status=STORES;
	
		if (ptr>=cpu->datasize && (ptr<MMIO || ptr>=MMIO+16))
		{
			status=NO_SUCH_DATA_MEMORY;
		}
        else
		{
			if (ptr>=MMIO && ptr<(MMIO+16)) mmio=TRUE;
			else mmio=FALSE;
		
			switch (opcode)
			{
			case I_SB:
				if (!mmio) cpu->dstat[ptr]=WRITTEN;
				b=(BYTE)cpu->rreg[rB].val;
				if (!mmio) *(BYTE *)(&cpu->data[ptr])=b;
				else *(BYTE *)(&cpu->mm[ptr-MMIO])=b;
				break;
			case I_SH:
				if (ptr%2!=0)
				{
					status=DATA_MISALIGNED;
					break;
				}
 
				if (!mmio) for (i=0;i<2;i++) cpu->dstat[ptr+i]=WRITTEN;
				h=(WORD16)cpu->rreg[rB].val;
				if (!mmio) *(WORD16 *)(&cpu->data[ptr])=h;
				else *(WORD16 *)(&cpu->mm[ptr-MMIO])=h;
				break;
			case I_SW:
				if (ptr%4!=0)
				{
					status=DATA_MISALIGNED;
					break;
				}
				if (!mmio) for (i=0;i<4;i++) cpu->dstat[ptr+i]=WRITTEN;
				w=(WORD32)cpu->rreg[rB].val;
				if (!mmio) *(WORD32 *)(&cpu->data[ptr])=w;
				else *(WORD32 *)(&cpu->mm[ptr-MMIO])=w;
				break;
			case I_SD:
			case I_S_D:
				if (ptr%8!=0)
				{
					status=DATA_MISALIGNED;
					break;
				}
				if (!mmio) for (i=0;i<8;i++) cpu->dstat[ptr+i]=WRITTEN;
				if (!mmio) *(WORD64 *)(&cpu->data[ptr])=cpu->rreg[rB].val;
				else *(WORD64 *)(&cpu->mm[ptr-MMIO])=cpu->rreg[rB].val;
				break;
			default:
				break;
			}
		}
        break;
    case REG3:
	case REG3X:
    case REG3F:
    case REG2S:
        if (condition && forwarding && ins.rd!=0) cpu->wreg[ins.rd].source=FROM_MEM;
        break;
    case REG1I:
    case REG2I:
	case REGDI:
        if (forwarding && ins.rt!=0) cpu->wreg[ins.rt].source=FROM_MEM;
		break;
	case REGID:
	case REG2F:
		if (forwarding && ins.rd!=0) cpu->wreg[ins.rd].source=FROM_MEM;
		break;
    case JUMP:
    case JREG:
        if (forwarding && (opcode==I_JAL || (opcode==I_SPECIAL && function==R_JALR)))
            cpu->wreg[31].source = FROM_MEM;
        break;
    default:
        break;
    }

    pipe->ex_mem.active=FALSE;
    pipe->mem_wb.active=TRUE;
    pipe->mem_wb.ALUOutput=pipe->ex_mem.ALUOutput;
    pipe->mem_wb.IR=pipe->ex_mem.IR;
    pipe->mem_wb.ins=ins;
    pipe->mem_wb.NPC=pipe->ex_mem.NPC;
	pipe->mem_wb.condition=pipe->ex_mem.condition;
    return status;
}

/*
   Write Back. Since writing takes place on the leading edge, update both the 
   read and write registers
*/

static int WB(pipeline *pipe,processor *cpu,BOOL forwarding)
{
    int status,type;
	BOOL condition;
    instruction ins=pipe->mem_wb.ins;

    if (!pipe->mem_wb.active) return EMPTY;

	condition=pipe->mem_wb.condition;
    type=ins.type;
    status=OK;
    switch (type)
    {
    case LOAD:
    case FLOAD:
        if (ins.rt==0) break;
        if (!forwarding)
        {
			make_available(cpu,ins.rt,pipe->mem_wb.LMD);
        }
        else
        { /* be careful in case a new value is now being forwarded */
          /* from further back... */
            if (cpu->rreg[ins.rt].source==FROM_MEM) 
                cpu->rreg[ins.rt].source=cpu->wreg[ins.rt].source=FROM_REGISTER;
        }
        break;
    case REG1I:
    case REG2I:
	case REGDI:
        if (ins.rt==0) break;
        if (!forwarding)
        {
			make_available(cpu,ins.rt,pipe->mem_wb.ALUOutput);
        }
        else
        {
            if (cpu->rreg[ins.rt].source==FROM_MEM)
                cpu->rreg[ins.rt].source=cpu->wreg[ins.rt].source=FROM_REGISTER;
        }
        break;
	case REGID:
	case REG2F:
        if (ins.rd==0) break;
        if (!forwarding)
        {
			make_available(cpu,ins.rd,pipe->mem_wb.ALUOutput);
        }
        else
        {
            if (cpu->rreg[ins.rd].source==FROM_MEM)
                cpu->rreg[ins.rd].source=cpu->wreg[ins.rd].source=FROM_REGISTER;
        }
		break;
    case REG3:
	case REG3X:
    case REG2S:
    case REG3F:
        if (ins.rd==0 || !condition) break;
        if (!forwarding)
        {
			make_available(cpu,ins.rd,pipe->mem_wb.ALUOutput);
        }
        else
        {
            if (cpu->rreg[ins.rd].source==FROM_MEM)
                cpu->rreg[ins.rd].source=cpu->wreg[ins.rd].source=FROM_REGISTER;
        }
        break;
    case JUMP:
        if (ins.opcode==I_JAL)
        {
            if (!forwarding)
            {
				make_available(cpu,31,pipe->mem_wb.NPC);
            }  
            else
            {
                if (cpu->rreg[31].source==FROM_MEM)
                    cpu->rreg[31].source=cpu->wreg[31].source=FROM_REGISTER;
            }
        }
        break;
    case JREG:
        if (ins.opcode==I_SPECIAL && ins.function==R_JALR)
        {
            if (!forwarding)
            { 
				make_available(cpu,31,pipe->mem_wb.NPC);
            }
            else
            {
                if (cpu->rreg[31].source==FROM_MEM)
                    cpu->rreg[31].source=cpu->wreg[31].source=FROM_REGISTER;
            }
        }
        break;
    case HALT:
        status=HALTED;
        break;
    default:
        break;
    }
    pipe->mem_wb.active=FALSE;
    return status;
}

int clock_tick(pipeline *pipe,processor *mips,BOOL forwarding,BOOL delay_slot,BOOL branch_target_buffer,RESULT *result)
{

    int i,status;
	BOOL empty;

/* activate WB first as it activates on leading edge */

    status=WB(pipe,mips,forwarding);
    if (status==HALTED || pipe->halting) 
	{ // check that pipeline is empty...
		pipe->halting=TRUE;
		empty=TRUE;
		for (i=0;i<pipe->MUL_LATENCY;i++) if (pipe->m[i].active) empty=FALSE;
		for (i=0;i<pipe->ADD_LATENCY;i++) if (pipe->a[i].active) empty=FALSE;
		if (pipe->div.active && pipe->div.cycles>0) empty=FALSE;
		if (pipe->ex_mem.active) empty=FALSE;
		if (pipe->mem_wb.active) empty=FALSE;
		if (empty) 	
		{
			result->WB=result->MEM=result->DIVIDER=result->EX=result->ID=result->IF=OK;
			for (i=0;i<pipe->MUL_LATENCY;i++) result->MULTIPLIER[i]=OK;
			for (i=0;i<pipe->ADD_LATENCY;i++) result->ADDER[i]=OK;


			return HALTED;           /* halted */
		}
	}
	
	result->WB=status;
    result->MEM=MEM(pipe,mips,forwarding,&result->memrr);

//	int extype=pipe->integer.ins.type;
//	if (extype==STORE || extype==FSTORE)
//	{ // in this case pull the store instruction through first
//		result->EX=EX_INT(pipe,mips,forwarding,&result->exrr);	
//		EX_MUL(pipe,mips,forwarding,&result->mulrr,result->MULTIPLIER);
//		EX_ADD(pipe,mips,forwarding,&result->addrr,result->ADDER);
//		result->DIVIDER=EX_DIV(pipe,mips,forwarding,&result->divrr);
//	}
//	else
//	{
		EX_MUL(pipe,mips,forwarding,&result->mulrr,result->MULTIPLIER);
		EX_ADD(pipe,mips,forwarding,&result->addrr,result->ADDER);
		result->DIVIDER=EX_DIV(pipe,mips,forwarding,&result->divrr);
		result->EX=EX_INT(pipe,mips,forwarding,&result->exrr);	
//	}
    result->ID=ID(pipe,mips,forwarding,branch_target_buffer,&result->idrr);
    result->IF=IF(pipe,mips,delay_slot,branch_target_buffer);

/* Copy Write to Read registers */
 
    for (i=0;i<64;i++) mips->rreg[i]=mips->wreg[i];

    return OK;
}
