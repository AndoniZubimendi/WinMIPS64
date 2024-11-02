#pragma once

//
// Useful types
//

#define FALSE 0
#define TRUE 1

#define READ 0
#define WRITE 1
#define BOTH 2

#define ENDLINE 1
#define SPACE   2
#define COMMENT 3

#define VACANT  0
#define WRITTEN 1

#define RUNNING 0
#define STOPPED 1

#define INT 0
#define ADD 1
#define MUL 2
#define DIV 3

// Stages

#define IFETCH  1
#define IDECODE 2
#define INTEX   3
#define ADDEX   4
#define MULEX   5
#define DIVEX   6
#define MEMORY  7
#define WRITEB  8

#define WHITE  RGB(255,255,255)
#define BLACK  RGB(0,0,0)
#define GREY   RGB(128,128,128)
#define YELLOW RGB(255,255,0)

#define CYAN   RGB(0,255,255)
#define RED    RGB(255,0,0)
#define GREEN  RGB(0,255,0)
#define BLUE   RGB(0,0,255)
#define MAGENTA   RGB(255,0,255)

#define DGREEN   RGB(0,128,0)
#define DCYAN    RGB(0,128,128)
#define DYELLOW  RGB(128,128,0)

#define DBLUE	 RGB(0,0,128)
#define DMAGENTA RGB(128,0,128)
#define DRED	 RGB(128,0,0)
#define LGREY    RGB(192,192,192)

typedef unsigned char BYTE;
typedef unsigned short WORD16;
typedef unsigned long WORD32;
typedef unsigned __int64 WORD64;

typedef short SIGNED16;
typedef long SIGNED32;
typedef __int64 SIGNED64;

typedef int BOOL;

union {
	WORD64 u;
	SIGNED64 s;
	double d;
} typedef DOUBLE64;

/* pipeline stage status */

#define NOT_A_BRANCH 0
#define BRANCH_TAKEN 1
#define BRANCH_NOT_TAKEN 2

#define OK			0

// stalls...

#define RAW			1
#define WAW			2 
#define STALLED		3
#define HALTED		4
#define STRUCTURAL	5
#define WAR         6
#define BRANCH_TAKEN_STALL 7
#define BRANCH_MISPREDICTED_STALL 8

// advisories....

#define DATA_ERR	9
#define EMPTY		10
#define DIVIDE_BY_ZERO   11
#define INTEGER_OVERFLOW 12
#define NO_SUCH_DATA_MEMORY   13
#define LOADS		14
#define STORES		15
#define NO_SUCH_CODE_MEMORY		16
#define DATA_MISALIGNED 17
#define WAITING_FOR_INPUT 18

/* register status */

#define NOT_AVAILABLE  0
#define FROM_REGISTER  1
#define FROM_MEM       2
#define FROM_EX        3
#define FROM_ID        4
#define FROM_ADD       5
#define FROM_MUL       6
#define FROM_DIV       7

/* significant instruction types */

#define R_TYPE 1
#define I_TYPE 2
#define J_TYPE 3
#define F_TYPE 4
#define M_TYPE 5
#define B_TYPE 6

#define NOP 0
#define LOAD  1
#define STORE 2
#define REG1I 3
#define REG2I 4
#define REG2S 5
#define JUMP  6
#define JREG  7
#define HALT  8
#define REG3F 9
#define BRANCH 10
#define REG3   11
#define REGID  12
#define FLOAD  13
#define FSTORE 14
#define JREGN  15
#define REG2F  16
#define REG3X  17
#define REGDI  18
#define REG2C  19
#define BC	   20

#define I_SPECIAL       0x00
#define I_COP1          0x11
#define I_DOUBLE        0x11
#define I_MTC1          0x04
#define I_MFC1          0x00
#define I_BC			0x08
#define I_HALT          0x01

#define I_J             0x02
#define I_JAL           0x03
#define I_BEQ           0x04
#define I_BNE           0x05
#define I_BEQZ          0x06
#define I_BNEZ          0x07

#define I_DADDI         0x18
#define I_DADDIU        0x19
#define I_SLTI          0x0A
#define I_SLTIU         0x0B
#define I_ANDI          0x0C
#define I_ORI           0x0D
#define I_XORI          0x0E
#define I_LUI           0x0F

#define I_LB            0x20
#define I_LH            0x21
#define I_LW            0x23
#define I_LBU           0x24
#define I_LHU           0x25
#define I_LWU           0x27
#define I_SB            0x28
#define I_SH            0x29
#define I_SW            0x2B
#define I_L_D           0x35
#define I_S_D           0x3D
#define I_LD            0x37
#define I_SD            0x3F

#define R_NOP           0x00
#define R_JR            0x08
#define R_JALR          0x09
#define R_MOVZ          0x0A
#define R_MOVN          0x0B

#define R_DSLLV         0x14
#define R_DSRLV         0x16
#define R_DSRAV         0x17
#define R_DMUL          0x1C
#define R_DMULU         0x1D
#define R_DDIV          0x1E
#define R_DDIVU         0x1F

#define R_AND           0x24
#define R_OR            0x25
#define R_XOR           0x26
#define R_SLT           0x2A
#define R_SLTU          0x2B
#define R_DADD          0x2C
#define R_DADDU         0x2D
#define R_DSUB          0x2E
#define R_DSUBU         0x2F

#define R_DSLL          0x38
#define R_DSRL          0x3A
#define R_DSRA          0x3B

#define F_ADD_D         0x00
#define F_SUB_D         0x01
#define F_MUL_D         0x02
#define F_DIV_D         0x03
#define F_MOV_D         0x06
#define F_CVT_D_L       0x21
#define F_CVT_L_D		0x25
#define F_C_LT_D        0x3C
#define F_C_LE_D        0x3E
#define F_C_EQ_D        0x32

typedef struct
{
    char *name;
    BYTE type;
    BYTE subtype;
    WORD32 op_code;
} op_code_info;

#define SI(x) ((WORD32)x<<26)
#define SR(x) ((WORD32)x | I_SPECIAL << 26)
#define SF(x) ((WORD32)x | I_COP1 << 26 | I_DOUBLE << 21)
#define SMTC1 (I_COP1 << 26 | I_MTC1 << 21)
#define SMFC1 (I_COP1 << 26 | I_MFC1 << 21)
#define SBC1F (I_COP1 << 26 | I_BC << 21)
#define SBC1T (I_COP1 << 26 | I_BC << 21 | 1 << 16)  

#define CODE 1
#define DATA 2

#define STEP 8
#define MASK -1

typedef struct
{
	int IF;
	int ID;
	int EX;
	int MEM;
	int WB;
	int DIVIDER;
	int ADDER[10];
	int MULTIPLIER[10];
	int idrr;
	int exrr;
	int memrr;
	int addrr;
	int mulrr;
	int divrr;

} RESULT;

typedef struct
{
    char *symb;
    BYTE type;
    WORD32 value;
} symbol_table;

struct
{
    int type,function,opcode,tf,target;
    int rs,rt,rd;
	int src1,src2;
    SIGNED32 Imm;
} typedef instruction;

struct
{
    WORD64 val;
    SIGNED32 source;
} typedef reg;

struct
{
	int    status;
	WORD32 codesize;
	WORD32 datasize;
    BYTE   *code;
	BYTE   *cstat;
    BYTE   *data;
	BYTE   *dstat;
	BYTE   mm[16];
	WORD32 *screen;
	WORD32 nlines;
	WORD32 ncols;
	BOOL drawit;
	CString Terminal;
	WORD32 keyboard;
    WORD32 PC;
    reg    rreg[64];
    reg    wreg[64];
	BOOL fp_cc;
} typedef processor;

struct 
{
    WORD32 IR;  /* pointer to instruction in memory */
    instruction ins;
    WORD32 NPC;
    BOOL active;
	BOOL predicted;
} typedef if_id_reg;

struct 
{
    WORD32 IR;
    instruction ins;
    int rA,rB;
    WORD32 NPC;
	WORD64 ALUOutput;
    SIGNED32 Imm;
    BOOL active;
    int cycles;
} typedef id_ex_reg;

struct 
{
    WORD32 IR;
    instruction ins;
    int rB;
    WORD64 ALUOutput;
    WORD32 NPC;
    BOOL active;
	BOOL condition;
} typedef ex_mem_reg;

struct
{
    WORD32 IR;
    instruction ins;
    WORD64 ALUOutput,LMD;
    WORD32 NPC;
    BOOL active;
	BOOL condition;
} typedef mem_wb_reg;

struct 
{
    BOOL active;
	BOOL halting;
    BOOL   branch;
    WORD32 destination;
	int ADD_LATENCY;
	int MUL_LATENCY;
	int DIV_LATENCY;

    if_id_reg  if_id;
    id_ex_reg integer;
    id_ex_reg m[10];
    id_ex_reg a[10];
    id_ex_reg div;
    ex_mem_reg ex_mem;
    mem_wb_reg  mem_wb;
} typedef pipeline;

struct
{
	BYTE stage;
	BYTE substage;
	BYTE cause;
} typedef entry;

struct
{
	WORD32 IR;
	WORD32 start_cycle;
	entry status[500];
} typedef record;

