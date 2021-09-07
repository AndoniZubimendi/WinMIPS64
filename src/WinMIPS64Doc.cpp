// WinMIPS64Doc.cpp : implementation of the CWinMIPS64Doc class
//

#include "stdafx.h"
#include "WinMIPS64.h"
#include <string.h>

#include "WinMIPS64Doc.h"
#include "utils.h"
#include "MemDialog.h"
#include "MultiDialog.h"
#include "MainFrm.h"
#include "IOView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_LINE 200

static char *directives[]=
{".space",
 ".asciiz",
 ".align",
 ".word",
 ".byte",
 ".ascii",
 ".global",
 ".data",
 ".text",
 ".org",
 ".word32",
 ".word16",
 ".double",
 ".code",
 NULL};

static op_code_info codes[]={
{"lb",     I_TYPE, LOAD,   SI(I_LB)},
{"lbu",    I_TYPE, LOAD,   SI(I_LBU)},
{"sb",     I_TYPE, STORE,  SI(I_SB)},
{"lh",     I_TYPE, LOAD,   SI(I_LH)},
{"lhu",    I_TYPE, LOAD,   SI(I_LHU)},
{"sh",     I_TYPE, STORE,  SI(I_SH)},
{"lw",     I_TYPE, LOAD,   SI(I_LW)},   
{"lwu",    I_TYPE, LOAD,   SI(I_LWU)},   
{"sw",     I_TYPE, STORE,  SI(I_SW)},
{"ld",     I_TYPE, LOAD,   SI(I_LD)},
{"sd",     I_TYPE, STORE,  SI(I_SD)},
{"l.d",    I_TYPE, FLOAD,  SI(I_L_D)},
{"s.d",    I_TYPE, FSTORE, SI(I_S_D)},
{"halt",   I_TYPE, HALT,   SI(I_HALT)},

{"daddi",  I_TYPE, REG2I,  SI(I_DADDI)},
{"daddui", I_TYPE, REG2I,  SI(I_DADDIU)},
{"andi",   I_TYPE, REG2I,  SI(I_ANDI)},
{"ori",    I_TYPE, REG2I,  SI(I_ORI)},
{"xori",   I_TYPE, REG2I,  SI(I_XORI)},
{"lui",    I_TYPE, REG1I,  SI(I_LUI)},

{"slti",   I_TYPE, REG2I,  SI(I_SLTI)},
{"sltiu",  I_TYPE, REG2I,  SI(I_SLTIU)},

{"beq",    I_TYPE, BRANCH,  SI(I_BEQ)},
{"bne",    I_TYPE, BRANCH,  SI(I_BNE)},
{"beqz",   I_TYPE, JREGN,   SI(I_BEQZ)},
{"bnez",   I_TYPE, JREGN,   SI(I_BNEZ)},


{"j",      J_TYPE, JUMP,   SI(I_J)},
{"jr",     R_TYPE, JREG,   SR(R_JR)},
{"jal",    J_TYPE, JUMP,   SI(I_JAL)},
{"jalr",   R_TYPE, JREG,   SR(R_JALR)},

{"dsll",   R_TYPE, REG2S,  SR(R_DSLL)},
{"dsrl",   R_TYPE, REG2S,  SR(R_DSRL)},
{"dsra",   R_TYPE, REG2S,  SR(R_DSRA)},
{"dsllv",  R_TYPE, REG3,   SR(R_DSLLV)},
{"dsrlv",  R_TYPE, REG3,   SR(R_DSRLV)},
{"dsrav",  R_TYPE, REG3,   SR(R_DSRAV)},
{"movz",   R_TYPE, REG3,   SR(R_MOVZ)},
{"movn",   R_TYPE, REG3,   SR(R_MOVN)},
{"nop",    R_TYPE, NOP,    SR(R_NOP)},
{"and",    R_TYPE, REG3,   SR(R_AND)},
{"or",     R_TYPE, REG3,   SR(R_OR)},
{"xor",    R_TYPE, REG3,   SR(R_XOR)},
{"slt",    R_TYPE, REG3,   SR(R_SLT)},
{"sltu",   R_TYPE, REG3,   SR(R_SLTU)},

{"dadd",   R_TYPE, REG3,   SR(R_DADD)},
{"daddu",  R_TYPE, REG3,   SR(R_DADDU)},
{"dsub",   R_TYPE, REG3,   SR(R_DSUB)},
{"dsubu",  R_TYPE, REG3,   SR(R_DSUBU)},

{"dmul",   R_TYPE, REG3,   SR(R_DMUL)},
{"dmulu",  R_TYPE, REG3,   SR(R_DMULU)},
{"ddiv",   R_TYPE, REG3,   SR(R_DDIV)},
{"ddivu",  R_TYPE, REG3,   SR(R_DDIVU)},

{"add.d",  F_TYPE, REG3F,   SF(F_ADD_D)},
{"sub.d",  F_TYPE, REG3F,   SF(F_SUB_D)},
{"mul.d",  F_TYPE, REG3F,   SF(F_MUL_D)},
{"div.d",  F_TYPE, REG3F,   SF(F_DIV_D)},
{"mov.d",  F_TYPE, REG2F,   SF(F_MOV_D)},
{"cvt.d.l",F_TYPE, REG2F,   SF(F_CVT_D_L)},
{"cvt.l.d",F_TYPE, REG2F,   SF(F_CVT_L_D)},
{"c.lt.d", F_TYPE, REG2C,   SF(F_C_LT_D)},
{"c.le.d", F_TYPE, REG2C,   SF(F_C_LE_D)},
{"c.eq.d", F_TYPE, REG2C,   SF(F_C_EQ_D)},

{"bc1f",   B_TYPE, BC,     SBC1F},
{"bc1t",   B_TYPE, BC,     SBC1T},
{"mtc1",   M_TYPE, REGID,  SMTC1},
{"mfc1",   M_TYPE, REGDI,  SMFC1},

{NULL,    0,      0,      0}
};

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64Doc

IMPLEMENT_DYNCREATE(CWinMIPS64Doc, CDocument)

BEGIN_MESSAGE_MAP(CWinMIPS64Doc, CDocument)
	//{{AFX_MSG_MAP(CWinMIPS64Doc)
	ON_COMMAND(ID_FILE_RESET, OnFileReset)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_EXECUTE_SINGLE, OnExecuteSingle)
	ON_COMMAND(ID_FILE_ARCH, OnFileMemory)
	ON_COMMAND(ID_EXECUTE_MULTICYCLE, OnExecuteMulticycle)
	ON_COMMAND(ID_FILE_MULTI, OnFileMulti)
	ON_COMMAND(ID_EXECUTE_RUNTO, OnExecuteRunto)
	ON_COMMAND(ID_EXECUTE_STOP, OnExecuteStop)
	ON_UPDATE_COMMAND_UI(ID_EXECUTE_STOP, OnUpdateExecuteStop)
	ON_UPDATE_COMMAND_UI(ID_EXECUTE_RUNTO, OnUpdateExecuteRunto)
	ON_UPDATE_COMMAND_UI(ID_EXECUTE_SINGLE, OnUpdateExecuteSingle)
	ON_UPDATE_COMMAND_UI(ID_EXECUTE_MULTICYCLE, OnUpdateExecuteMulticycle)
	ON_UPDATE_COMMAND_UI(ID_FILE_MULTI, OnUpdateFileMulti)
	ON_UPDATE_COMMAND_UI(ID_FILE_ARCH, OnUpdateFileMemory)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_RESET, OnUpdateFileReset)
	ON_COMMAND(ID_FULL_RESET, OnFullReset)
	ON_UPDATE_COMMAND_UI(ID_FULL_RESET, OnUpdateFullReset)
	ON_COMMAND(ID_RELOAD, OnReload)
	ON_UPDATE_COMMAND_UI(ID_RELOAD, OnUpdateReload)
	ON_COMMAND(ID_FILE_DELAY_SLOT, OnFileDelaySlot)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELAY_SLOT, OnUpdateFileDelaySlot)
	ON_COMMAND(ID_FILE_FORWARDING, OnFileForwarding)
	ON_UPDATE_COMMAND_UI(ID_FILE_FORWARDING, OnUpdateFileForwarding)
	ON_COMMAND(ID_BTB, OnBtb)
	ON_UPDATE_COMMAND_UI(ID_BTB, OnUpdateBtb)
	ON_COMMAND(ID_FILE_REGISTER_AS_NUMBER, OnFileRegistersAsNumbers)
	ON_UPDATE_COMMAND_UI(ID_FILE_REGISTER_AS_NUMBER, OnUpdateRegistersAsNumbers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64Doc construction/destruction

CWinMIPS64Doc::CWinMIPS64Doc()
{
	unsigned int i,codebits,databits;
	char txt[12];
	int val;
	CString fname;
	CStdioFile file;
	codebits=10;
	databits=10;
	CODESIZE=0x400; // defaults
	DATASIZE=0x400;
	ADD_LATENCY=4;
	MUL_LATENCY=7;
	DIV_LATENCY=24;
	forwarding=TRUE;
	delay_slot=FALSE;
	entries=1;
	offset=0;
	history[0].IR=0;
	history[0].start_cycle=0;
	history[0].status[0].stage=IFETCH;

	GetCurrentDirectory(MAX_PATH,AppDir); // where I am running from?

 	fname=(CString)AppDir+'\\'+"winmips64.pth";
	if (file.Open(fname,CFile::modeRead))
	{
		file.ReadString(AppDir,MAX_PATH);
		file.Close();
	}
	strcpy_s(LasDir,MAX_PATH+1, AppDir); 	

 	fname=(CString)AppDir+'\\'+"winmips64.ini";

	txt[0]=txt[11]=0;
	if (file.Open(fname,CFile::modeRead))
	{
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_CODEBITS && val>=MIN_CODEBITS) codebits=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_DATABITS && val>=MIN_DATABITS) databits=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_ADD_LATENCY && val>=MIN_ADD_LATENCY)	ADD_LATENCY=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_MUL_LATENCY && val>=MIN_MUL_LATENCY) MUL_LATENCY=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_DIV_LATENCY && val>=MIN_DIV_LATENCY) DIV_LATENCY=val;
		file.ReadString(txt,10);
		if (atoi(txt)==1)	delay_slot=TRUE;
		else				delay_slot=FALSE;
		file.ReadString(txt,10);
		if (atoi(txt)==1)	forwarding=TRUE;
		else				forwarding=FALSE;
		file.ReadString(txt,10);
		if (atoi(txt)==1 && !delay_slot)	branch_target_buffer=TRUE;
		else				                branch_target_buffer=FALSE;
		file.ReadString(txt,10);
		if (atoi(txt)==1 && !registers_as_numbers)	registers_as_numbers=TRUE;
		else				                registers_as_numbers=FALSE;
		

		file.Close();
		CODESIZE=(1<<codebits);
		DATASIZE=(1<<databits);
	}

	lastfile="";
	fname=(CString)AppDir+'\\'+"winmips64.las";

	if (file.Open(fname,CFile::modeRead))
	{
		file.ReadString(lastfile);
		file.Close();
	}

	init_processor(&cpu,CODESIZE,DATASIZE);
	init_pipeline(&pipe,ADD_LATENCY,MUL_LATENCY,DIV_LATENCY);

	cpu.code=new BYTE[CODESIZE];
	cpu.cstat=new BYTE[CODESIZE];
	cpu.data=new BYTE[DATASIZE];
	cpu.dstat=new BYTE[DATASIZE];
	cpu.screen=new WORD32[GSXY*GSXY];

	codelines=new CString[CODESIZE/4];
	assembly=new CString[CODESIZE/4];
	mnemonic=new CString[CODESIZE/4];
	datalines=new CString[DATASIZE/8];

	for (i=0;i<CODESIZE;i++) cpu.code[i]=cpu.cstat[i]=0;
	for (i=0;i<DATASIZE;i++) cpu.data[i]=cpu.dstat[i]=0;
	for (i=0;i<16;i++) cpu.mm[i]=0;
	for (i=0;i<GSXY*GSXY;i++) cpu.screen[i]=WHITE;
	cpu.Terminal=""; cpu.nlines=0; cpu.drawit=FALSE; cpu.keyboard=0;

	cpu.status=RUNNING;
	simulation_running=FALSE;
	restart=FALSE;

	clear();
	OnReload();

}

CWinMIPS64Doc::~CWinMIPS64Doc()
{
	CStdioFile file;
	CString fname;
	char txt[10];
	fname=(CString)AppDir+'\\'+"winmips64.ini";

	if (file.Open(fname,CFile::modeCreate | CFile::modeWrite))
	{
		sprintf_s(txt,10, "%d\n",bits(CODESIZE));
		file.WriteString(txt);
		sprintf_s(txt,10,"%d\n",bits(DATASIZE));
		file.WriteString(txt);
		sprintf_s(txt,10,"%d\n",ADD_LATENCY);
		file.WriteString(txt);
		sprintf_s(txt,10,"%d\n",MUL_LATENCY);
		file.WriteString(txt);
		sprintf_s(txt,10,"%d\n",DIV_LATENCY);
		file.WriteString(txt);
		if (delay_slot) sprintf_s(txt,10,"1\n");
		else			sprintf_s(txt,10,"0\n");
		file.WriteString(txt);
		if (forwarding) sprintf_s(txt,10,"1\n");
		else			sprintf_s(txt,10,"0\n");
		file.WriteString(txt);
		if (branch_target_buffer) sprintf_s(txt,10,"1\n");
		else			          sprintf_s(txt,10,"0\n");
		file.WriteString(txt);
		if (registers_as_numbers) sprintf_s(txt,10,"1\n");
		else			          sprintf_s(txt,10,"0\n");
		file.WriteString(txt);

	
		file.Close();
	}
	delete cpu.code;
	delete cpu.data;
	delete cpu.cstat;
	delete cpu.dstat;
	delete cpu.screen;
	delete [] codelines;
	delete [] assembly;
	delete [] mnemonic;
	delete [] datalines;
}

void CWinMIPS64Doc::clear()
{
	cycles=instructions=loads=stores=branch_taken_stalls=branch_misprediction_stalls=0;
	raw_stalls=waw_stalls=war_stalls=structural_stalls=0;
	cpu.PC=0;
	entries=1;
	offset=0;
	history[0].IR=0;
	history[0].start_cycle=0;
	history[0].status[0].stage=IFETCH;
	multi=5;
	stall_type=stalls=0;
}

BOOL CWinMIPS64Doc::OnNewDocument()
{

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64Doc serialization

void CWinMIPS64Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64Doc diagnostics

#ifdef _DEBUG
void CWinMIPS64Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinMIPS64Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64Doc commands


void CWinMIPS64Doc::OnFileReset() 
{ // Reset the processor
	unsigned int i;

	init_pipeline(&pipe,ADD_LATENCY,MUL_LATENCY,DIV_LATENCY);

	cpu.PC=0;
	for (i=0;i<64;i++) 
	{
		cpu.rreg[i].val=cpu.wreg[i].val=0;
		cpu.rreg[i].source=cpu.wreg[i].source=FROM_REGISTER;
	}
	for (i=0;i<CODESIZE;i++) cpu.cstat[i]=0;
	cpu.status=RUNNING;
	clear();
	for (i=0;i<16;i++) cpu.mm[i]=0;
	for (i=0;i<GSXY*GSXY;i++) cpu.screen[i]=WHITE;
	cpu.Terminal=""; cpu.nlines=0; cpu.drawit=FALSE; cpu.keyboard=0;
	cpu.fp_cc=FALSE;

	UpdateAllViews(NULL);

}

void CWinMIPS64Doc::OnFullReset() 
{ // Reset Data Memory as well
	unsigned i;
	for (i=0;i<DATASIZE/8;i++) datalines[i]="";
	for (i=0;i<DATASIZE;i++) cpu.data[i]=cpu.dstat[i]=0;

	OnFileReset();	
}

void CWinMIPS64Doc::OnFileOpen() 
{	
	CFileDialog dlg(TRUE,"s","*.s");
	CStdioFile last;
	CString fname,path;

	fname=(CString)LasDir+'\\'+"winmips64.las";

	if (last.Open(fname,CFile::modeRead))
	{
		last.ReadString(path);
		dlg.m_ofn.lpstrInitialDir=path.Left(path.ReverseFind('\\'));
		last.Close();
	}

	if (dlg.DoModal()!=IDOK) return;
	path=dlg.GetPathName();

	int res=openfile(path);

	if (res==0)
	{

// AfxMessageBox(fname,MB_OK);
		if (last.Open(fname,CFile::modeCreate | CFile::modeWrite))
		{
// create .las file in directory from which application was run
			lastfile=path;
			last.WriteString(lastfile);
			last.Close();
		}
		else
		{
// if unable to write it there -  write it here instead
			int pathstart=path.ReverseFind('\\');
			if (pathstart>=0 && pathstart<MAX_PATH)
				strcpy_s(LasDir,MAX_PATH, path.Left(pathstart));
			else
				LasDir[0]=0;
			fname=(CString)LasDir+'\\'+"winmips64.las";
			if (last.Open(fname,CFile::modeCreate | CFile::modeWrite))
			{
// create .las file in user directory
				lastfile=path;
				last.WriteString(lastfile);
				last.Close();
			}
		}
	}

	UpdateAllViews(NULL,1L);
}

void CWinMIPS64Doc::check_stalls(int status,char *str,int rawreg,char *txt)
{
	char mess[100];
	if (status==RAW)	
	{
		raw_stalls++;
		if (rawreg<32)
			sprintf_s(mess,100,"  Atasco RAW en %s (R%d)",str,rawreg);
		else
			sprintf_s(mess,100,"  Atasco RAW en %s (F%d)",str,rawreg-32);
		strcat_s(txt,100, mess);
	}
	if (status==WAW)	
	{
		waw_stalls++;
		if (rawreg<32)
			sprintf_s(mess,100,"  Atasco WAW en %s (R%d)",str,rawreg);
		else
			sprintf_s(mess,100,"  Atasco WAW en %s (F%d)",str,rawreg-32);
		strcat_s(txt,100,mess);
	}
	if (status==WAR)
	{
		war_stalls++;
		if (rawreg<32)
			sprintf_s(mess,100,"  Atasco WAR en %s (R%d)",str,rawreg);
		else
			sprintf_s(mess,100,"  Atasco WAR en %s (F%d)",str,rawreg-32);
		strcat_s(txt,100,mess);
	}
}

void CWinMIPS64Doc::process_result(RESULT *result,BOOL show)
{
	char txt[300];
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	BOOL something=FALSE;


	if (result->WB==OK || result->WB==HALTED) instructions++;
	txt[0]=0;
	if (!delay_slot && result->ID==BRANCH_TAKEN_STALL)
	{
		something=TRUE;
		branch_taken_stalls++;
		strcat_s(txt,100,"  Atasco Branch Taken");
	}
	if (result->ID==BRANCH_MISPREDICTED_STALL)
	{
		something=TRUE;
		branch_misprediction_stalls++;
		strcat_s(txt,100,"  Atasco Branch Misprediction");
	}

	if (result->MEM==LOADS || result->MEM==DATA_ERR) loads++;
	if (result->MEM==STORES) stores++;

	check_stalls(result->ID,"ID",result->idrr,txt);
	check_stalls(result->EX,"EX",result->exrr,txt);
	check_stalls(result->ADDER[0],"ADD",result->addrr,txt);
	check_stalls(result->MULTIPLIER[0],"MUL",result->mulrr,txt);
	check_stalls(result->DIVIDER,"DIV",result->divrr,txt);
	check_stalls(result->MEM,"MEM",result->memrr,txt);

	if (result->MEM!=RAW)
	{
		if (result->ID==STALLED)
		{
			structural_stalls++;
			strcat_s(txt,300,"  Atasco Estructural en ID");
		}
		if (result->EX==STALLED)
		{
			structural_stalls++;
			strcat_s(txt,300,"  Atasco Estructural en EX");
		}
		if (result->DIVIDER==STALLED)
		{
			structural_stalls++;
			strcat_s(txt,300,"  Atasco Estructural en FP-DIV");
		}
		if (result->MULTIPLIER[MUL_LATENCY-1]==STALLED)
		{
			structural_stalls++;
			strcat_s(txt,300,"  Atasco Estructural en FP-MUL");
		}
		if (result->ADDER[ADD_LATENCY-1]==STALLED)
		{
			structural_stalls++;
			strcat_s(txt,300,"  Atasco Estructural en FP-ADD");
		}
	}
	if (result->IF==NO_SUCH_CODE_MEMORY)
	{
		strcat_s(txt,300,"  No existe esa dirección de código!");
		cpu.status=HALTED;
	}
	if (result->EX==INTEGER_OVERFLOW)
	{
		strcat_s(txt,300,"  Desbordamiento de número entero!");
	}
	if (result->DIVIDER==DIVIDE_BY_ZERO)
	{
		strcat_s(txt,300,"  División por Cero en DIV!");
	}

	if (result->MEM==DATA_ERR)
	{
		strcat_s(txt,300,"  Memoria no inicializada en MEM!");
	}
	if (result->MEM==NO_SUCH_DATA_MEMORY)
	{
		strcat_s(txt,300,"  No existe esa dirección de datos!");
	}
	if (result->MEM==DATA_MISALIGNED)
	{
		strcat_s(txt, 300," Error Fatal - LOAD/StTORE de memoria mal alineado!");
	}

	if (show)
	{
		if (txt[0]==0) pStatus->SetPaneText(0,"Listo");
		else pStatus->SetPaneText(0,txt);
	}
}

int CWinMIPS64Doc::update_io(processor *cpuarg)
{
	WORD32 func=*(WORD32 *)&cpuarg->mm[0];
	int i,x,y,nlines,ncols,status=0;

	if (!func) return status;

	char txt[30];
	DOUBLE64 fp;
	fp.u=*(WORD64 *)&cpuarg->mm[8]; 

	switch (func)
	{
	case (WORD32)1:
		sprintf_s(txt,30,"%I64u\n",fp.u);
		cpuarg->Terminal+=txt;
		UpdateAllViews(NULL,2);
		break;
	case (WORD32)2:
		sprintf_s(txt,30,"%I64d\n",fp.s);
		cpuarg->Terminal+=txt;
		UpdateAllViews(NULL,2);
		break;
	case (WORD32)3:
		sprintf_s(txt,30,"%lf\n",fp.d);
		cpuarg->Terminal+=txt;
		UpdateAllViews(NULL,2);
		break;
	case (WORD32)4:
// need to test here if fp.u is a legal address!
        if (fp.u<cpuarg->datasize) {
        	cpuarg->Terminal = cpuarg->Terminal + &cpuarg->data[fp.u];
        }
        UpdateAllViews(NULL,2);
		break;

	case (WORD32)5:

		y=(DWORD32)((fp.u>>32)&255);
		x=(DWORD32)((fp.u>>40)&255);
		cpuarg->drawit=TRUE;
//			char txt[80];
//			sprintf(txt,"%d %d",x,y);
//			AfxMessageBox(txt);

		if (x<GSXY && y<GSXY)
		{
			cpuarg->screen[GSXY*y+x]=(WORD32)fp.u;
		}
		UpdateAllViews(NULL,2);
		break;
	case (WORD32)6:
		cpuarg->Terminal="";
		cpuarg->nlines=0; 
		UpdateAllViews(NULL,2);
		break;
	case (WORD32)7:
		for (i=0;i<GSXY*GSXY;i++) cpuarg->screen[i]=WHITE;
		cpuarg->drawit=FALSE;
		UpdateAllViews(NULL,2);
		break;
	case (WORD32)8:
		cpuarg->keyboard=1;
		status=1;
		break;
	case (WORD32)9:
		cpuarg->keyboard=2;
		status=1;
		break;
	default:
		break;
	}

	nlines=0;
	ncols=0;
	for (i=0;i<cpuarg->Terminal.GetLength();i++)
	{
		if (cpuarg->Terminal[i]=='\n')
		{
			nlines++;
			ncols=0;
		}
		else
			ncols++;
	}
	cpuarg->nlines=nlines;
	cpuarg->ncols=ncols;
	
//	UpdateAllViews(NULL,1L);

	*(WORD32 *)&cpuarg->mm[0]=0;
	return status;
}

void CWinMIPS64Doc::update_history(pipeline *pipearg,processor *cpuarg,RESULT *result)
{
	int substage,stage;
	unsigned int i,cc;
	WORD32 previous;
	BOOL passed;

	if (result->MEM!=RAW)
	{
		if (result->ID==STALLED) result->ID=STRUCTURAL;
		if (result->EX==STALLED) result->EX=STRUCTURAL;
		if (result->DIVIDER==STALLED) result->DIVIDER=STRUCTURAL;
		if (result->MULTIPLIER[MUL_LATENCY-1]==STALLED) result->MULTIPLIER[MUL_LATENCY-1]=STRUCTURAL;
		if (result->ADDER[ADD_LATENCY-1]==STALLED) result->ADDER[ADD_LATENCY-1]=STRUCTURAL;
	}	

	for (i=0;i<entries;i++)
	{
		previous=history[i].IR;
		cc=cycles-history[i].start_cycle;
		stage=history[i].status[cc-1].stage; // previous stage
		substage=history[i].status[cc-1].substage;
	
		switch (stage)
		{

		case IFETCH:
			if (pipearg->if_id.active)
			{
				if (pipearg->if_id.IR==previous)
				{

					history[i].status[cc].stage=IDECODE;
					history[i].status[cc].cause=0;
				}
				else
				{
					history[i].status[cc].stage=IFETCH;
					history[i].status[cc].cause= (BYTE) result->IF;
				}
			}
			else 
			{
				history[i].status[cc].stage=0;
				history[i].status[cc].cause=0;
			}
			break;
		case IDECODE:
			passed=FALSE;
			
			if (pipearg->integer.active && pipearg->integer.IR==previous && result->ID!=STALLED)
			{
				passed=TRUE;
				history[i].status[cc].stage=INTEX;
				history[i].status[cc].cause=0;
			}
			if (pipearg->m[0].active && pipearg->m[0].IR==previous && result->ID!=STALLED)
			{
				passed=TRUE;
				history[i].status[cc].stage=MULEX;
				history[i].status[cc].substage=0;
				history[i].status[cc].cause=0;
			}
			if (pipearg->a[0].active && pipearg->a[0].IR==previous && result->ID!=STALLED)
			{
				passed=TRUE;
				history[i].status[cc].stage=ADDEX;
				history[i].status[cc].substage=0;
				history[i].status[cc].cause=0;
			}
			if (pipearg->div.active && pipearg->div.IR==previous && result->ID!=STALLED)
			{
				passed=TRUE;
				history[i].status[cc].stage=DIVEX;
				history[i].status[cc].cause=0;
			}
			
			if (!passed) 
			{
				history[i].status[cc].stage=IDECODE;
				history[i].status[cc].cause=(BYTE) result->ID;
			}
			break;
		case INTEX:
			if (pipearg->ex_mem.active && pipearg->ex_mem.IR==previous)
			{
				history[i].status[cc].stage=MEMORY;
				history[i].status[cc].cause=0;
			}
			else
			{
				history[i].status[cc].stage=INTEX;
				history[i].status[cc].cause=(BYTE) result->EX;
			}
			break;

		case MULEX:
			if (substage==pipearg->MUL_LATENCY-1)
			{
				if (pipearg->ex_mem.active && pipearg->ex_mem.IR==previous)
				{
					history[i].status[cc].stage=MEMORY;
					history[i].status[cc].cause=0;
				}
				else
				{
					history[i].status[cc].stage=MULEX;
					history[i].status[cc].substage= (BYTE) substage;
					history[i].status[cc].cause= (BYTE) result->MULTIPLIER[MUL_LATENCY-1];
				}
			}
			else
			{
				if (pipearg->m[substage+1].active && pipearg->m[substage+1].IR==previous)
				{
					history[i].status[cc].stage=MULEX;
					history[i].status[cc].substage= (BYTE) (substage + 1);
					history[i].status[cc].cause=0;
				}
				else 
				{
					history[i].status[cc].stage=MULEX;
					history[i].status[cc].substage=(BYTE) substage;
					history[i].status[cc].cause=(BYTE) result->MULTIPLIER[substage];
				}
			}
			break;

		case ADDEX:
			if (substage==pipearg->ADD_LATENCY-1)
			{
				if (pipearg->ex_mem.active && pipearg->ex_mem.IR==previous)
				{
					history[i].status[cc].stage=MEMORY;
					history[i].status[cc].cause=0;
				}
				else
				{
					history[i].status[cc].stage=ADDEX;
					history[i].status[cc].substage=(BYTE) substage;
					history[i].status[cc].cause=(BYTE) result->ADDER[ADD_LATENCY-1];
				}
			}
			else
			{
				if (pipearg->a[substage+1].active && pipearg->a[substage+1].IR==previous)
				{
					history[i].status[cc].stage=ADDEX;
					history[i].status[cc].substage=(BYTE) (substage + 1);
					history[i].status[cc].cause=0;
				}
				else 
				{
					history[i].status[cc].stage=ADDEX;
					history[i].status[cc].substage=(BYTE) substage;
					history[i].status[cc].cause= (BYTE) result->ADDER[substage];
				}
			}
			break;
		case DIVEX:
			if (pipearg->ex_mem.active && pipearg->ex_mem.IR==previous)
			{
				history[i].status[cc].stage=MEMORY;
				history[i].status[cc].cause=0;
			}
			else
			{
				history[i].status[cc].stage=DIVEX;
				history[i].status[cc].cause=(BYTE) result->DIVIDER;
			}
			break;

		case MEMORY:
			if (pipearg->mem_wb.active && pipearg->mem_wb.IR==previous)
			{
				history[i].status[cc].stage=WRITEB;
				history[i].status[cc].cause=0;
			}
			else
			{
				history[i].status[cc].stage=MEMORY;
				history[i].status[cc].cause=(BYTE) result->MEM;
			}
			break;

		case WRITEB:
			history[i].status[cc].stage=0;
			history[i].status[cc].cause=0;
			break;

		default:
			history[i].status[cc].stage=0;
			history[i].status[cc].cause=0;
		}
	}

// make a new entry
//	if (cpu->PC!=history[entries-1].IR)
	if ((result->ID==OK || result->ID==EMPTY || cpuarg->PC!=history[entries-1].IR) && pipearg->active)
	{
		history[entries].IR=cpuarg->PC;
		history[entries].status[0].stage=IFETCH;
		history[entries].status[0].cause=0;
		history[entries].start_cycle=cycles;
		entries++;
	}
	if (entries==50)
	{
		entries--;
		for (i=0;i<entries;i++)
		{
			history[i]=history[i+1];
		}
	}

}

int CWinMIPS64Doc::one_cycle(pipeline *pipearg,processor *cpuarg,BOOL show)
{
	int status=0;
	RESULT result;

	if (cpuarg->status==HALTED) return HALTED;

	status=clock_tick(pipearg,cpuarg,forwarding,delay_slot,branch_target_buffer,&result);

	cycles++;
	process_result(&result,show);
	update_history(pipearg,cpuarg,&result);
	if (update_io(cpuarg)) return WAITING_FOR_INPUT;
	if (status==HALTED) 
	{
		cpuarg->status=HALTED;
		return HALTED;
	}

	return status;
}

void CWinMIPS64Doc::OnExecuteSingle() 
{
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	int status=one_cycle(&pipe,&cpu,TRUE);
	if (status==WAITING_FOR_INPUT)
	{
		pStatus->SetPaneText(0,"Esperando Entrada");
	}
	UpdateAllViews(NULL,1L);	// send hint that								
								// code window should be scrolled
}

void CWinMIPS64Doc::OnExecuteMulticycle() 
{
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	int i;
	int status = 0;
	simulation_running=TRUE;
	for (i=0;i<multi-1;i++)
	{
		status = one_cycle(&pipe,&cpu,FALSE);
		if (status) break;
	}
	if (status==0) status= one_cycle(&pipe,&cpu,TRUE); // show status after last one.

	if (status==WAITING_FOR_INPUT)
	{
		pStatus->SetPaneText(0,"Esperando Entrada");
	}

	simulation_running=FALSE;
	UpdateAllViews(NULL,1L);	// send hint that	
								// code window should be scrolled
}

void CWinMIPS64Doc::OnExecuteStop() 
{
	simulation_running=FALSE;
}

void CWinMIPS64Doc::OnExecuteRunto() 
{
	MSG message;
	char buf[80];
	int status,lapsed=0;
	simulation_running=TRUE;
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	pStatus->SetPaneText(0,"Ejecutando Simulación");
	do
	{
		if (::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		lapsed++;
		status = one_cycle(&pipe,&cpu,FALSE);
		if (status) break;
	} while (stalls || ((cpu.cstat[cpu.PC]&1)==0 && cpu.status!=HALTED && simulation_running));
	simulation_running=FALSE;
	if (status==WAITING_FOR_INPUT) 
	{
		sprintf_s(buf,80,"Simulación Detenida luego de %d ciclos - Esperando Entrada",lapsed);
		restart=TRUE;
	}
	else
	{
		sprintf_s(buf,80,"Simulación Detenida luego de %d ciclos",lapsed);
		restart=FALSE;
	}

	pStatus->SetPaneText(0,buf);
	UpdateAllViews(NULL,1L);	// send hint that		
								// code window should be scrolled
}

int CWinMIPS64Doc::openfile(CString fname)
{
	unsigned int i;
	int res;
	OnFileReset();
	for (i=0;i<DATASIZE;i++) cpu.data[i]=cpu.dstat[i]=0; // reset memory
	for (i=0;i<CODESIZE;i++) cpu.code[i]=cpu.cstat[i]=0;
	for (i=0;i<16;i++) cpu.mm[i]=0;
	for (i=0;i<GSXY*GSXY;i++) cpu.screen[i]=WHITE;
	cpu.Terminal=""; cpu.nlines=0; cpu.drawit=FALSE; cpu.keyboard=0;
	
	for (i=0;i<CODESIZE/4;i++) 
	{
		codelines[i]="";
		assembly[i]="";
		mnemonic[i]="";
	}
	for (i=0;i<DATASIZE/STEP;i++) datalines[i]="";

	if (fname=="") return 1;

	res=openit(fname);
	if (res>0)
	{
		remove("winmips64.ini");
		remove("winmips64.las"); 
		AfxGetMainWnd()->SetWindowText("WinMIPS64 - Simulador de Procesador MIPS64");
		lastfile="";
	}
	if (res==1)
	{
		char txt[512];
		CT2A fname_ascii(fname);
		sprintf_s(txt,512,"No se pudo abrir el archivo %s",fname_ascii.m_psz);
		AfxMessageBox(txt,MB_OK|MB_ICONEXCLAMATION);
		return res;
	}
	if (res==2)
	{
		AfxMessageBox("Errores detectados en 1ra Pasada");
		return res;
	}
	if (res==3)
	{
		AfxMessageBox("Errores detectados en 2da Pasada");
		return res;
	}
	AfxGetMainWnd()->SetWindowText("WinMIPS64 - Simulador de Procesador MIPS64 - " + fname);

	return res;
}

//
// This function reads a line even if not termimated by CR
//

int CWinMIPS64Doc::mygets(char *line,int max,CFile *fp)
{
	int i,bytes;
	char ch;
	i=0;
	do
	{
		bytes=fp->Read(&ch,1);
		if (i==0 && bytes==0) return 1;
		if (bytes==0) break;
		if (i<max-3) line[i++]=ch;
		else
		{
            line[i++]='\n';
            line[i]='\0';
            do
            {
                bytes=fp->Read(&ch,1);
            } while (bytes!=0 && ch!='\n' && ch!='\0');
            
            return -1;
        }

	} while (ch!='\n' && ch!='\0');		
	if (ch!='\n') line[i++]='\n'; // put in a CR if not one already
	line[i]='\0';
	return 0;
}

int CWinMIPS64Doc::openit(CString fname)
{
	int i,j,k,gotline,lineptr,errors;
	char preline[MAX_LINE+1];
	char line[MAX_LINE+1];
	CStdioFile asmfile;
	
	errors=0;
	CODEORDATA=0;
	codeptr=0;
	dataptr=0;

	code_symptr=0;
	data_symptr=0;

	if (!asmfile.Open(fname,CFile::modeRead)) return 1;
	for (lineptr=1;;lineptr++)
	{
//		if (!asmfile.ReadString(line,80)) break; // ***
		gotline=mygets(line,MAX_LINE,&asmfile);
		if (gotline==1) break;
		if (gotline==-1) 
        { // something got chopped
            errors++;
        }
		errors+=first_pass(line,lineptr);
		if (errors) break;
		dataptr=alignment(dataptr,STEP);
	}
	asmfile.Close();
	CODEORDATA=0;
	codeptr=0;
	dataptr=0;

	if (errors) return 2; // errors on pass 1

	errors=0;
	asmfile.Open(fname,CFile::modeRead);
	for (lineptr=1;;lineptr++)
	{
//		if (!asmfile.ReadString(preline,80)) break; // ***
		if (mygets(preline,MAX_LINE,&asmfile)==1) break; // ***
		preline[strlen(preline)-1]=0; // remove CR/LF
		
		for (i=j=0;;i++)
		{ // replace Tabs with 2 spaces
			if (preline[i]=='\t')
				for (k=0;k<2;k++) line[j++]=' ';
			else
				line[j++]=preline[i];

			if (preline[i]==0) 
			{ // pad it out with spaces...
//char txt[100];
//sprintf(txt,"%d",j);
//AfxMessageBox(txt);
				line[j-1]=' ';
				for (;j<MAX_LINE;j++) line[j]=' ';
				line[MAX_LINE]=0;
				break;
			}
		}

		errors+=second_pass(line,lineptr);
	
		dataptr=alignment(dataptr,STEP);
	}
//	AfxMessageBox("Leaving openit");
	asmfile.Close();
	if (errors) return 3; // errors on pass 2
	return 0;
}

BOOL CWinMIPS64Doc::getcodesym(char *&ptr,WORD32 *m)
{
    return getsym(code_table,code_symptr,ptr,m);
}

BOOL CWinMIPS64Doc::getdatasym(char *&ptr,WORD32 *m)
{
    return getsym(data_table,data_symptr,ptr,m);
}

int CWinMIPS64Doc::instruction(char *start)
{
    int i=0;
    char text[10];    /* all instructions are 6 chars or less */
    char *ptr=start;
    if (ptr==NULL) return (-1); 
    while (i<10 && !delimiter(*ptr)) 
    {
        text[i++]= (unsigned char) tolower(*ptr);
        ptr++;
    }
    if (i>9) return (-1);
    text[i]=0;    /* terminate it */

    for (i=0;;i++)
    {
        if (codes[i].name==NULL) return (-1);
        if (strcmp(text,codes[i].name)==0) break;
    }
    return i;
}

BOOL CWinMIPS64Doc::directive(int pass,char *ptr,char *line)
{ // process assembler directives. return number of bytes consumed
    
    BOOL zero,bs;
	WORD32 num,m;
	WORD64 fw;
	DOUBLE64 db;
    int k,i;
    int sc,ch;
    BYTE b[10];
    char *iptr;

    if (ptr==NULL || *ptr!='.') return FALSE;
    
    for (k=0;;k++)
    {
        if (directives[k]==NULL) return FALSE;
        if (!compare(ptr,directives[k]))  continue;
        break;
    }
    while (!delimiter(*ptr)) ptr++;
    zero=TRUE;
    switch (k)
    {
    case 0:     // .space
        if (CODEORDATA==CODE) return FALSE;
        if (!getnum(ptr,&num)) return FALSE;
        if (num==0) return FALSE;
		if (CODEORDATA==DATA)
		{
			if (pass==2 && dataptr<=DATASIZE)
				datalines[dataptr/STEP]=(CString)line;
			dataptr+=num;
		//	if (dataptr>DATASIZE) return FALSE;
		}
		
        return TRUE;
        break;
    case 5:     // .ascii
        zero=FALSE;
    case 1:     // .asciiz
        if (CODEORDATA==CODE) return FALSE;
        ptr=eatwhite(ptr);
		if (ptr==NULL) return (-1);
        if (*ptr!='"' && *ptr!='\'') return (-1);        
        sc=*ptr;    // character to indicate end of string
        *ptr++;
        num=0;
        iptr=ptr;
		bs=FALSE;
        while (*iptr!=sc)
        {
            if (delimiter(*iptr)==ENDLINE) return FALSE;
            
		    if (bs)
            {
                num++;
                bs=FALSE;
            }
            else
            {
                if (*ptr=='\\') bs=TRUE;
                else num++;
            }
 
			
			iptr++;
        }
        if (zero) num++;              // trailing 0 needed
		if (CODEORDATA==DATA)
		{
			if (pass==1) 
			{
				dataptr+=num;
				if (dataptr>DATASIZE) return FALSE;
			}
			if (pass==2)
			{
				datalines[dataptr/STEP]=(CString)line;
                      
				if (zero) *iptr=0;     // stuff in a zero
				m=0;
				bs=FALSE;
				while (m<num)
				{              
					m++; 
                    if (bs)
                    {
                        if (*ptr=='n') ch='\n';
                        else ch=*ptr;
						cpu.dstat[dataptr]=WRITTEN;
						cpu.data[dataptr++]= (BYTE) ch;
                        bs=FALSE;
                    }
                    else
                    {
                        if (*ptr=='\\') bs=TRUE;
                        else
                        {
							cpu.dstat[dataptr]=WRITTEN;
							cpu.data[dataptr++]= *ptr;
                        }
                    } 
  
					
					ptr++;
				}
			}  
		}

        return TRUE;
    case 2:            // .align
        if (!getnum(ptr,&num)) return FALSE;
        if (num<2 || num>16) return FALSE;
		if (CODEORDATA==CODE) return FALSE;
		if (CODEORDATA==DATA)
		{
			dataptr=alignment(dataptr,num);
			if (pass==2)
				datalines[dataptr/STEP]=(CString)line;
		}
		
        return TRUE;
    case 3:            // .word
        if (CODEORDATA==CODE) return FALSE;
        if (pass==1)
        {
			if (CODEORDATA==DATA)
			{
				do {
					dataptr+=STEP;
				}
				while ((ptr=skipover(ptr,','))!=NULL);

				if (dataptr>DATASIZE) return FALSE;
			}
			
        }
        if (pass==2)
        {
            if (!getfullnum(ptr,&fw)) return FALSE;
			if (CODEORDATA==DATA)
			{
				datalines[dataptr/STEP]=(CString)line;

			//    printf("%08x %08x %s",dataptr,num,line);
				unpack(fw,b);
				for (i=0;i<STEP;i++) 
				{
					cpu.dstat[dataptr]=WRITTEN;
					cpu.data[dataptr++]=b[i];
				}
				while ((ptr=skip(ptr,','))!=NULL)
				{
					if (!getfullnum(ptr,&fw)) return FALSE;
            //   printf("         %08x\n",num);
					unpack(fw,b);
					for (i=0;i<STEP;i++) 
					{
						cpu.dstat[dataptr]=WRITTEN;
						cpu.data[dataptr++]=b[i];
					}
				}
			}
			
        }
        return TRUE;

    case 10:            // .word32
        if (CODEORDATA==CODE) return FALSE;
        if (pass==1)
        {
			if (CODEORDATA==DATA)
			{
				do {
					dataptr+=4;
				}
				while ((ptr=skipover(ptr,','))!=NULL);
				if (dataptr>DATASIZE) return FALSE;
			}
        }
        if (pass==2)
        {
            if (!getdatasym(ptr,&num)) return FALSE;
			if (CODEORDATA==DATA)
			{
				datalines[dataptr/STEP]=(CString)line;
				unpack32(num,b);
				for (i=0;i<4;i++) 
				{
					cpu.dstat[dataptr]=WRITTEN;
					cpu.data[dataptr++]=b[i];
				}
				while ((ptr=skip(ptr,','))!=NULL)
				{
					if (!getdatasym(ptr,&num)) return FALSE;
					unpack32(num,b);
					for (i=0;i<4;i++) 
					{
						cpu.dstat[dataptr]=WRITTEN;
						cpu.data[dataptr++]=b[i];
					}
				}
			}
        }
        return TRUE;
    case 11:            // .word16
        if (CODEORDATA==CODE) return FALSE;
        if (pass==1)
        {
			if (CODEORDATA==DATA)
			{
				do {
					dataptr+=2;
				}
				while ((ptr=skipover(ptr,','))!=NULL);
				if (dataptr>DATASIZE) return FALSE;
			}
        }
        if (pass==2)
        {
            if (!getnum(ptr,&num)) return FALSE;
            if (!in_range(num,0xffff)) return FALSE;
			if (CODEORDATA==DATA)
			{
				datalines[dataptr/STEP]=(CString)line;
				unpack16((WORD16)num,b);
				for (i=0;i<2;i++) 
				{
					cpu.dstat[dataptr]=WRITTEN;
					cpu.data[dataptr++]=b[i];
				}
				while ((ptr=skip(ptr,','))!=NULL)
				{
					if (!getnum(ptr,&num)) return FALSE;
					if (!in_range(num,0xffff)) return FALSE;
					unpack16((WORD16)num,b);
					for (i=0;i<2;i++) 
					{
						cpu.dstat[dataptr]=WRITTEN;
						cpu.data[dataptr++]=b[i];
					}
				}
			}
        }
        return TRUE;

    case 4:            // .byte
        if (CODEORDATA==CODE) return FALSE;
        if (pass==1)
        {
			if (CODEORDATA==DATA)
			{
				do {
					dataptr+=1;
				}
				while ((ptr=skipover(ptr,','))!=NULL);
				if (dataptr>DATASIZE) return FALSE;
			}
        }
        if (pass==2)
        {
            if (!getnum(ptr,&num)) return FALSE;
            if (!in_range(num,0xff)) return FALSE;   
			if (CODEORDATA==DATA)
			{
				datalines[dataptr/STEP]=(CString)line;
				cpu.dstat[dataptr]=WRITTEN;
				cpu.data[dataptr++]=(unsigned char)num;
				while ((ptr=skip(ptr,','))!=NULL)
				{
					if (!getnum(ptr,&num)) return FALSE;
					if (!in_range(num,0xff)) return FALSE;
					cpu.dstat[dataptr]=WRITTEN;
					cpu.data[dataptr++]=(unsigned char)num;
				}
			}
        }
        return TRUE;
    case 6:      // .global
    case 7:      // .data
        CODEORDATA=DATA;
		if (pass==1)
		{
			if (eatwhite(ptr)!=NULL) return FALSE;
		}
        return TRUE;
	case 12:	// double (64 bits)
        if (CODEORDATA==CODE) return FALSE;
        if (pass==1)
        {
			if (CODEORDATA==DATA)
			{
				do {
					dataptr+=STEP;
				}
				while ((ptr=skipover(ptr,','))!=NULL);
				if (dataptr>DATASIZE) return FALSE;
			}
			
        }
        if (pass==2)
        {
            if (!getdouble(ptr,&db.d)) return FALSE;
			if (CODEORDATA==DATA)
			{
				datalines[dataptr/STEP]=(CString)line;

			//    printf("%08x %08x %s",dataptr,num,line);
				unpack(db.u,b);
				for (i=0;i<STEP;i++) 
				{
					cpu.dstat[dataptr]=WRITTEN;
					cpu.data[dataptr++]=b[i];
				}
				while ((ptr=skip(ptr,','))!=NULL)
				{
					if (!getdouble(ptr,&db.d)) return FALSE;
            //   printf("         %08x\n",num);
					unpack(db.u,b);
					for (i=0;i<STEP;i++) 
					{
						cpu.dstat[dataptr]=WRITTEN;
						cpu.data[dataptr++]=b[i];
					}
				}
			}
			
        }		
		return TRUE;

	case 13:
	case 8:      // .text
        CODEORDATA=CODE;
		if (pass==1)
		{
			if (eatwhite(ptr)!=NULL) return FALSE;
		}
        return TRUE;
 
	case 9:    // .org
        if (CODEORDATA==DATA)
        {
            if (!getnum(ptr,&num)) return FALSE;
            if (num<dataptr /* || num>DATASIZE*/ ) return FALSE;
           
            dataptr=alignment(num,STEP);
      //      if (pass==2)
      //          printf("%08x          %s",dataptr,line);
            return TRUE;
        }
        if (CODEORDATA==CODE)
        {
            if (!getnum(ptr,&num)) return FALSE;
            if (num<codeptr || num>CODESIZE) return FALSE;
            codeptr=alignment(num,4);
       //     if (pass==2)
       //         printf("%08x          %s",codeptr,line);
            return TRUE;
        }
        return FALSE;

    default:
        break;
    }
    return FALSE;
}

// fill in symbol tables and check for syntax errors

int CWinMIPS64Doc::first_pass(char *line,int lineptr)
{
    int i,len;
    char *ptr=line;
	char txt[100];
	
    ptr=eatwhite(ptr);
    if (ptr==NULL) return 0;
    if (delimiter(*ptr)) return 0;  /* blank line */

    while ((len=is_symbol(ptr))>0)
    {
      //  copy symbol into symbol table
      //  advance over symbol
        
        if (CODEORDATA==0)
        {
            sprintf_s(txt,100,"Pasada 1 - Error en linea %d\n",lineptr);
			AfxMessageBox(txt);
            return 1;
        }
        if (CODEORDATA==CODE)
        {
            if (code_symptr>=SYMTABSIZE)
            {
                sprintf_s(txt,100,"Pasada 1 - Error en linea %d\nLa tabla de Simbolos de Código esta llena.",lineptr);
                AfxMessageBox(txt);
                return 1;
            } 
            code_table[code_symptr].symb=(char *)malloc(len+1);
            for (i=0;i<len;i++) code_table[code_symptr].symb[i]=*ptr++;
            code_table[code_symptr].symb[len]=0;
            code_table[code_symptr].value=codeptr;
            ptr++;           // skip over the ":"
            code_symptr++;
        }
        if (CODEORDATA==DATA)
        {
            if (data_symptr>=SYMTABSIZE)
            {
                sprintf_s(txt,100,"Pasada 1 - Error en linea %d\nLa tabla de Simbolos de Datos esta llena.",lineptr);
                AfxMessageBox(txt);
                return 1;
            } 
            data_table[data_symptr].symb=(char *)malloc(len+1);
            for (i=0;i<len;i++) data_table[data_symptr].symb[i]=*ptr++;
            data_table[data_symptr].symb[len]=0;
            data_table[data_symptr].value=dataptr;
            ptr++;           // skip over the ":"
            data_symptr++;
        }
        ptr=eatwhite(ptr);
        if (ptr==NULL) return 0;
    }
    if (instruction(ptr)>=0)
    {
        // instruction found
        // increase instruction pointer
        if (CODEORDATA!=CODE)
        {
            sprintf_s(txt,100,"Pasada 1 - Error en linea %d\n",lineptr);
			AfxMessageBox(txt);
            return 1;
        }
		codeptr=alignment(codeptr,4);
        codeptr+=4;
        if (codeptr>CODESIZE)
        {
            sprintf_s(txt,100,"Pasada 1 - Error en linea %d\nNo existe esa ubicación de memoria",lineptr);
            AfxMessageBox(txt);
            return 1;
        }
        return 0;
    }
    if (directive(1,ptr,line)) return 0;

    sprintf_s(txt,100,"Pasada 1 - Error en linea %d\n",lineptr);
    AfxMessageBox(txt);
    return 1;
}

int CWinMIPS64Doc::second_pass(char *line,int /* lineptr */)
{
    WORD32 w,byte;
    WORD32 op,code_word=0;
	WORD32 flags=0;
    BYTE b[4];
    int i,instruct;
	char *start,*end,*fin;
    int rs,rt,rd,sub,type;
    BOOL sign,error=TRUE;
    
    char *ptr=line;
    ptr=eatwhite(ptr);
    if (ptr==NULL) 
    {
    //    printf("                  %s",line);
        return 0;  /* blank line */
    }

// skip over any symbols on the line
    while (is_symbol(ptr))
    {
        while (*ptr!=':') ptr++;
        ptr++;
        ptr=eatwhite(ptr);
        if (ptr==NULL) break;
    } 
//AfxMessageBox(line);
    instruct=instruction(ptr);
    if (instruct<0)
    { // no instruction on the line, perhaps a directive?
        
        if (!directive(2,ptr,line))
        {
        //   if (CODEORDATA==CODE) printf("%08x          %s",codeptr,line);
			if (ptr!=NULL && CODEORDATA==DATA) return 1;
        }
        return 0;
    }

// instruction found - parse it out
	start=ptr;
    op=codes[instruct].op_code;
    sub=codes[instruct].subtype;
    type=codes[instruct].type;
    while (!delimiter(*ptr)) ptr++; // advance over instruction
//    ptr=eatwhite(ptr); 
	fin=ptr;
    rs=rt=rd=0;
    w=0; byte=0; flags=0;
    sign=TRUE;

    switch (sub)
    {
  case NOP:
  case HALT:
	    if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;    
   
  case STORE:
  case LOAD:
        rt=getreg(ptr);    
		if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over ,  */
		ptr=eatwhite(ptr);
		if (*ptr=='(') w=0;
        else if (!getdatasym(ptr,&w)) break;
        ptr=skip(ptr,'('); if (eatwhite(ptr)==NULL) break;  /* skip over (  */
		ptr=eatwhite(ptr);
        rs=getreg(ptr);    if (rs<0) break;
 
		ptr=skip(ptr,')');                                  /* skip over ) */
		if (ptr==NULL) break;
		if (eatwhite(ptr)!=NULL) break;

        error=FALSE;
        break;    
  case FSTORE:
  case FLOAD:
        rt=fgetreg(ptr);    if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
		if (*ptr=='(') w=0;
        else if (!getdatasym(ptr,&w)) break;
        ptr=skip(ptr,'('); if (eatwhite(ptr)==NULL) break;  /* skip over ( */
		ptr=eatwhite(ptr);
        rs=getreg(ptr);    if (rs<0) break;
        ptr=skip(ptr,')');                                  /* skip over ) */
		if (ptr==NULL) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;    

  case REG2I:
        rt=getreg(ptr); if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        rs=getreg(ptr); if (rs<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
		if (!getdatasym(ptr,&w)) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;    

  case REG1I:
        rt=getreg(ptr); if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        if (!getdatasym(ptr,&w)) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;    

  case JREG:
        rt=getreg(ptr); if (rt<0) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;    

  case REG3:
        rd=getreg(ptr); if (rd<0) break; 
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over ,  */
		ptr=eatwhite(ptr);
        rs=getreg(ptr); if (rs<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over ,  */
		ptr=eatwhite(ptr);
        rt=getreg(ptr); if (rt<0) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;

  case REG3F:
        rd=fgetreg(ptr); if (rd<0) break; 
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over ,  */
		ptr=eatwhite(ptr);
        rs=fgetreg(ptr); if (rs<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over ,  */
		ptr=eatwhite(ptr);
        rt=fgetreg(ptr); if (rt<0) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;

  case REG2F:
        rd=fgetreg(ptr); if (rd<0) break; 
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        rs=fgetreg(ptr); if (rs<0) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;

 case REG2C:
        rs=fgetreg(ptr); if (rs<0) break; 
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        rt=fgetreg(ptr); if (rt<0) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;

  case REGID:
  case REGDI:
        rt=getreg(ptr); if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        rd=fgetreg(ptr); if (rd<0) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;
  
  case REG2S:
        rd=getreg(ptr); if (rd<0) break; 
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        rs=getreg(ptr); if (rs<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;  /* skip over , */
		ptr=eatwhite(ptr);
        if (!getdatasym(ptr,&flags)) break;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;
       
  case JUMP:
  case BC:

		if (!getcodesym(ptr,&w)) break;
        w-=(codeptr+4);   /* relative jump */
        w=(SIGNED32)w/4;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;    

  case BRANCH:
        rt=getreg(ptr); if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;
		ptr=eatwhite(ptr);
        rs=getreg(ptr); if (rs<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;
		ptr=eatwhite(ptr);
        if (!getcodesym(ptr,&w)) break;    
        w-=(codeptr+4);
        w=(SIGNED32)w/4;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;

  case JREGN:
        rt=getreg(ptr); if (rt<0) break;
        ptr=skip(ptr,','); if (eatwhite(ptr)==NULL) break;
		ptr=eatwhite(ptr);
        if (!getcodesym(ptr,&w)) break;    
        w-=(codeptr+4);
        w=(SIGNED32)w/4;
		if (eatwhite(ptr)!=NULL) break;
        error=FALSE;
        break;

  default: 
        error=FALSE;
        break;
    }
    
    if (!error) switch (type)
    {
   case I_TYPE:
        if (in_range(w,0xffff)) 
            code_word=(op | rs<<21 | rt<<16 | (w&0xffff));
        else error=TRUE;   
        break;
   case R_TYPE:
        if (in_range(flags,0x1F))
            code_word=(op | rs<<21 | rt << 16 | rd<<11 | flags<<6);
        else error=TRUE;
        break;
   case J_TYPE:
        if (in_range(w,0x3ffffff)) code_word=(op | w&0x3ffffff);
        else error=TRUE;
        break;
   case F_TYPE:
        code_word=(op | rs << 11 | rt << 16 | rd << 6);
        break;
   case M_TYPE:
        code_word=(op | rt << 16 | rd << 11);
        break;
   case B_TYPE:
        if (in_range(w,0xffff)) code_word=(op | w&0xffff);
        else error=TRUE;
		break;
    }

	codeptr=alignment(codeptr,4);

	int ret_val=0;
    if (error) 
    {
       // printf("%08x ???????? %s",codeptr,line);
       // for (i=0;i<4;i++) cpu.code[codeptr++]=0x00;
		code_word=0;
		cpu.cstat[codeptr]=4;
        ret_val=1;     
    }

	if (ptr==NULL)
	{ // its crap...
		assembly[codeptr/4]="";
		mnemonic[codeptr/4]="";
	}
	else
	{
		end=ptr;

		int len=end-start;
		if (len>25) len=25;

		CString str(start,len);
		assembly[codeptr/4]=str;
	
		len=fin-start;

		if (len>7) len=7;

		CString str1(start,len);
		str1.MakeLower();
		mnemonic[codeptr/4]=str1;
	}

	codelines[codeptr/4]=(CString)line;
    // printf("%08x %08x %s",codeptr,code_word,line);
    unpack32(code_word,b);
    for (i=0;i<4;i++) cpu.code[codeptr++]=b[i];

    return ret_val;    
}

void CWinMIPS64Doc::OnFileMulti() 
{
	CMultiDialog dlg;

	dlg.m_cycles=multi;
	dlg.DoModal();
    multi=dlg.m_cycles;	
}

void CWinMIPS64Doc::OnFileMemory() 
{
	unsigned int i;
	unsigned int codesize,datasize;
	CMemDialog dlg;

	dlg.m_code=bits(CODESIZE);
	dlg.m_data=bits(DATASIZE);
	dlg.m_fpal=ADD_LATENCY;
	dlg.m_fpml=MUL_LATENCY;
	dlg.m_fpdl=DIV_LATENCY;

	if (dlg.DoModal()!=IDOK) return;

	codesize=(1<<dlg.m_code);
	datasize=(1<<dlg.m_data);
						// full system reset
	CODESIZE=codesize;
	DATASIZE=datasize;
	ADD_LATENCY=dlg.m_fpal;
	MUL_LATENCY=dlg.m_fpml;
	DIV_LATENCY=dlg.m_fpdl;

	delete cpu.code;
	delete cpu.data;
	delete cpu.cstat;
	delete cpu.dstat;
	delete cpu.screen;
	delete [] codelines;
	delete [] datalines;
	delete [] assembly;
	delete [] mnemonic;

	init_processor(&cpu,CODESIZE,DATASIZE);
	init_pipeline(&pipe,ADD_LATENCY,MUL_LATENCY,DIV_LATENCY);

	forwarding=TRUE;
	delay_slot=FALSE;
	branch_target_buffer=FALSE;

	cpu.code=new BYTE[CODESIZE];
	cpu.data=new BYTE[DATASIZE];
	cpu.dstat=new BYTE[DATASIZE];
	cpu.cstat=new BYTE[CODESIZE];
	cpu.screen=new WORD32[GSXY*GSXY];
	codelines=new CString[CODESIZE/4];
	assembly=new CString[CODESIZE/4];
	mnemonic=new CString[CODESIZE/4];
	datalines=new CString[DATASIZE/STEP];
	
	for (i=0;i<CODESIZE;i++) cpu.code[i]=cpu.cstat[i]=0;
	for (i=0;i<DATASIZE;i++) cpu.data[i]=cpu.dstat[i]=0;
	for (i=0;i<16;i++) cpu.mm[i]=0;
	for (i=0;i<GSXY*GSXY;i++) cpu.screen[i]=WHITE;
	cpu.Terminal=""; cpu.nlines=0; cpu.drawit=FALSE; cpu.keyboard=0;

	clear();
	cpu.status=RUNNING;
	UpdateAllViews(NULL);

	OnReload();
}

// Disable everything while simulation is running

void CWinMIPS64Doc::OnUpdateExecuteMulticycle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running && !cpu.keyboard);	
}

void CWinMIPS64Doc::OnUpdateExecuteStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(simulation_running);	
}

void CWinMIPS64Doc::OnUpdateExecuteRunto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running && !cpu.keyboard);
}

void CWinMIPS64Doc::OnUpdateExecuteSingle(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running && !cpu.keyboard);
}

void CWinMIPS64Doc::OnUpdateFileMulti(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running);	
}

void CWinMIPS64Doc::OnUpdateFileMemory(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running);
}

void CWinMIPS64Doc::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running);
}

void CWinMIPS64Doc::OnUpdateFileReset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running);	
}

void CWinMIPS64Doc::OnUpdateFullReset(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running);	
}

void CWinMIPS64Doc::OnReload() 
{ // reload last file
	int res;
	char txt[512];
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	res=openfile(lastfile);
	if (res==0) 
	{
	//	AfxGetMainWnd()->SetWindowText(lastfile);
		CT2A lastfilee_ascii(lastfile);
		sprintf_s(txt, 512, "Archivo cargado - %s", lastfilee_ascii.m_psz);
		pStatus->SetPaneText(0,txt);
	}
}

void CWinMIPS64Doc::OnUpdateReload(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!simulation_running);
	
}

void CWinMIPS64Doc::OnFileDelaySlot() 
{
	if (delay_slot) delay_slot=FALSE;
	else			{delay_slot=TRUE;}
	OnFileReset();
}

void CWinMIPS64Doc::OnUpdateFileDelaySlot(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(delay_slot);
	pCmdUI->Enable(!branch_target_buffer);
}

void CWinMIPS64Doc::OnFileForwarding() 
{
	if (forwarding) forwarding=FALSE;
	else            forwarding=TRUE;
	OnFileReset();
}

void CWinMIPS64Doc::OnUpdateFileForwarding(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(forwarding);	
}

void CWinMIPS64Doc::OnBtb() 
{
	if (branch_target_buffer) branch_target_buffer=FALSE;
	else			          {branch_target_buffer=TRUE;}
	OnFileReset();
}

void CWinMIPS64Doc::OnUpdateBtb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(branch_target_buffer);
	pCmdUI->Enable(!delay_slot);
}

void CWinMIPS64Doc::OnFileRegistersAsNumbers() 
{
	if (registers_as_numbers) registers_as_numbers=FALSE;
	else            registers_as_numbers=TRUE;
	UpdateAllViews(NULL);

}

void CWinMIPS64Doc::OnUpdateRegistersAsNumbers(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(registers_as_numbers);	
}
