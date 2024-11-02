#include "stdafx.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "utils.h"

/* Architectural globals */

int bits(int num)
{
	int r = 0;
	while (num >>= 1) r++;
	return r;
}

// print out nbits, rounded up to nearest byte


// TODO: should pass txt size
int sprintnbits(char* txt, WORD32 addr, int n)
{
	int i, nbytes = n / 8;
	if (n % 8 != 0) nbytes++;
	BYTE a[4];
	unpack32(addr, a);
	for (i = 0; i < nbytes; i++) sprintf_s(&txt[2 * i], 3, "%02x", a[nbytes - i - 1]);
	return nbytes;
}


SIGNED16 pack16(BYTE* b)
{ /* pack bytes into 16-bit half-word */
	SIGNED16 res = (SIGNED16)b[0];
	res |= (SIGNED16)(b[1] << 8);
	return res;
}

void unpack16(SIGNED16 a, BYTE* b)
{ /* unpack bytes from a half-word */
	b[0] = (BYTE)a;
	b[1] = (BYTE)(a >> 8);
}

WORD32 pack32(BYTE* b)
{ /* pack bytes into a 32-bit Word */
	return ((WORD32)b[3] << 24) | ((WORD32)b[2] << 16) | ((WORD32)b[1] << 8) | (WORD32)b[0];
}

void unpack32(WORD32 a, BYTE* b)
{ /* unpack bytes from a word */
	b[0] = (BYTE)a;
	b[1] = (BYTE)(a >> 8);
	b[2] = (BYTE)(a >> 16);
	b[3] = (BYTE)(a >> 24);
}

WORD64 pack(BYTE* b)
{
	int i;
	WORD64 r = 0;
	for (i = STEP - 1; i >= 0; i--)
	{
		r <<= 8;
		r |= b[i];
	}
	return r;
}

void unpack(WORD64 a, BYTE* b)
{
	int i;
	for (i = 0; i < STEP; i++)
	{
		b[i] = (BYTE)a;
		a >>= 8;
	}
}

// TODO: should pass txt size
void sprintword32(char* txt, WORD32 addr)
{
	sprintf_s(txt, 9, "%08x", addr);
}


// TODO: should pass txt size
void sprintword(char* txt, WORD64 val)
{
	int i;
	BYTE a[8];
	val &= MASK;
	unpack(val, a);
	for (i = 0; i < STEP; i++) sprintf_s(&txt[2 * i], 3, "%02x", a[STEP - i - 1]);
}

void sprintdouble(char* txt, double db)
{
	sprintf_s(txt, 26, "%016.8lf", db);
}

BOOL in_range(WORD32 num, WORD32 mask)
{ // check that num will fit into the number of bits in mask
	int n = num;
	if (n >= 0 && (num & mask) != num) return FALSE;
	if (n < 0 && (num | mask) != -1)  return FALSE;
	return TRUE;
}

/* align objects on num-byte boundaries */

int alignment(int ptr, int num)
{
	int t, r = ptr;
	t = r % num;
	if (t > 0) r += (num - t);
	return r;
}

int delimiter(int c)
{
	if (c == ';' || c == '#') return COMMENT;
	if (c == 0 || c == '\n') return ENDLINE;
	if (c == ' ' || c == 9)  return SPACE;
	return 0;
}

int compare(char* ptr1, char* ptr2)
{ // compare strings, up to 0 in the second with all of the first
	int incr = 0;
	if (ptr1 == NULL) return 0;
	while (*ptr2 == *ptr1)
	{
		if (*ptr2 == 0) break;
		ptr1++; ptr2++; incr++;
	}
	if (*ptr2 == 0 && !isalnum(*ptr1)) return incr;
	return 0;

}

char* skipover(char* ptr, char c)
{ // skip over anything to get to c

	int res;
	while (*ptr != c)
	{
		res = delimiter(*ptr);
		if (res == ENDLINE || res == COMMENT) return NULL;
		if (c == ',' && *ptr == '(') return NULL;
		ptr++;
	}
	ptr++;

	return ptr;
}

char* skip(char* ptr, char c)
{ // skip over next c

	ptr = eatwhite(ptr);
	if (ptr == NULL) return NULL;
	if (*ptr != c) return NULL;
	ptr++;
	return ptr;

	/*
		int res;
		while (*ptr!=c)
		{
			res=delimiter(*ptr);
			if (res==ENDLINE || res==COMMENT) return NULL;
			if (c==',' && *ptr=='(') return NULL;
			ptr++;
		}
		ptr++;

		return ptr;
	*/
}

int is_symbol(char* start)
{
	int len = 0;
	char* ptr = start;
	while (!delimiter(*ptr)) { ptr++; len++; }
	ptr--; len--;
	if (*ptr == ':') return len;
	return 0;
}

char* eatwhite(char* ptr)
{
	if (ptr == NULL) return NULL;

	while (*ptr == ' ' || *ptr == 9) ptr++;
	if (*ptr == 0 || *ptr == ';' || *ptr == '#' || *ptr == '\n') return NULL;
	return ptr;
}

int getreg(char*& ptr)
{
	int n, ch, type;
	ptr = eatwhite(ptr);
	if (ptr == NULL) return (-1);
	ch = tolower(*ptr);
	if (ch != 'r' && ch != '$') return (-1);

	ptr++;
	if (ch == 'r' && !isdigit(*ptr)) return (-1);

	/* check for register pseudo-name */
	type = 0;
	if (strncmp(ptr, "zero", 4) == 0) { n = 0; ptr += 4; return n; }
	if (strncmp(ptr, "at", 2) == 0) { n = 1; ptr += 2; return n; }
	if (strncmp(ptr, "gp", 2) == 0) { n = 28; ptr += 2; return n; }
	if (strncmp(ptr, "sp", 2) == 0) { n = 29; ptr += 2; return n; }
	if (strncmp(ptr, "fp", 2) == 0) { n = 30; ptr += 2; return n; }
	if (strncmp(ptr, "ra", 2) == 0) { n = 31; ptr += 2; return n; }
	if (strncmp(ptr, "v", 1) == 0) { type = 1; ptr++; }
	if (strncmp(ptr, "a", 1) == 0) { type = 2; ptr++; }
	if (strncmp(ptr, "t", 1) == 0) { type = 3; ptr++; }
	if (strncmp(ptr, "s", 1) == 0) { type = 4; ptr++; }
	if (strncmp(ptr, "k", 1) == 0) { type = 5; ptr++; }


	n = (int)atoi(ptr);
	ptr++;
	while (isdigit(*ptr)) ptr++;

	switch (type)
	{
	case 1:
		if (n < 0 || n>1) return -1;
		n += 2;
		break;
	case 2:
		if (n < 0 || n>3) return -1;
		n += 4;
		break;
	case 3:
		if (n < 0 || n>9) return -1;
		if (n < 8) n += 8;
		else n += 16;
		break;
	case 4:
		if (n < 0 || n>7) return -1;
		n += 16;
		break;
	case 5:
		if (n < 0 || n>1) return -1;
		n += 26;
		break;

	default: break;
	}

	while (n < 0 || n>31) return (-1);

	return n;
}

int fgetreg(char*& ptr)
{
	int n;
	ptr = eatwhite(ptr);
	if (ptr == NULL) return (-1);
	if (tolower(*ptr) != 'f') return (-1);
	ptr++;
	if (!isdigit(*ptr)) return (-1);
	n = (int)atoi(ptr);
	ptr++;
	while (isdigit(*ptr)) ptr++;
	if (n < 0 || n>31) return (-1);
	return n;
}

WORD64 strtoint64(char* ptr, char** end, int b)
{
	WORD64 n = 0;
	int ch, s = 0;
	int base = 10;

	while (*ptr == ' ' || *ptr == 9) ptr++;
	if (*ptr == '-') { s = 1; ptr++; }
	else if (*ptr == '+') ptr++;
	while (*ptr == ' ' || *ptr == 9) ptr++;

	if (b > 0) base = b;
	else
	{
		if (*ptr == '0')
		{
			ptr++;
			if (*ptr == 'x' || *ptr == 'X')
			{
				base = 16;
				ptr++;
			}
			else
				base = 8;
		}
	}
	for (;;)
	{
		ch = *ptr;
		if (base == 8)
		{
			if (ch < '0' || ch>'7') break;
			n = n * base + (ch - '0');
		}
		if (base == 10)
		{
			if (ch < '0' || ch>'9') break;
			n = n * base + (ch - '0');
		}
		if (base == 16)
		{
			if ((ch < '0' || ch>'9') && (ch < 'A' || ch>'F') && (ch < 'a' || ch>'f'))
				break;
			if (ch >= '0' && ch <= '9') n = n * base + (ch - '0');
			if (ch >= 'A' && ch <= 'F') n = n * base + 10 + (ch - 'A');
			if (ch >= 'a' && ch <= 'f') n = n * base + 10 + (ch - 'a');
		}
		ptr++;
	}

	if (end != NULL)	*end = ptr;

	if (s == 1) return ((~n) + 1);

	return n;
}

BOOL getfullnum(char*& ptr, WORD64* num)
{
	WORD64 m;
	*num = 0;
	ptr = eatwhite(ptr);
	if (ptr == NULL) return FALSE;
	//   if (*ptr=='#') ptr++;   // skip over #
	if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+') return FALSE;

	m = strtoint64(ptr, &ptr, 0);

	*num = m;

	return TRUE;
}

BOOL getdouble(char*& ptr, double* num)
{
	double m;
	ptr = eatwhite(ptr);
	if (ptr == NULL) return FALSE;
	//  if (*ptr=='#') ptr++;   /* skip over # */
	if (!isdigit(*ptr) && *ptr != '.' && *ptr != '-' && *ptr != '+') return FALSE;

	m = strtod(ptr, &ptr);

	*num = m;

	return TRUE;
}

BOOL getnum(char*& ptr, WORD32* n)
{
	int sign = 1;
	ptr = eatwhite(ptr);
	if (ptr == NULL) return FALSE;
	//    if (*ptr=='#') ptr++;   // skip over #
	if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+') return FALSE;

	if (*ptr == '-') { sign = -1; ptr++; }
	if (*ptr == '+') ptr++;

	*n = (int)strtoul(ptr, &ptr, 0);

	*n *= sign;

	return TRUE;
}

/*
   get symbol from a symbol table
   This should use a hash table....
*/

BOOL getsym(symbol_table* table, int size, char*& ptr, WORD32* n)
{
	int k, incr;
	WORD32 m;
	char* sptr;
	ptr = eatwhite(ptr);
	if (ptr == NULL) return FALSE;
	if (getnum(ptr, &m))
	{ // its a number!
		*n = m;
		return TRUE;
	}

	for (k = 0; k < size; k++)
	{
		sptr = table[k].symb;
		incr = compare(ptr, sptr);

		if (incr)
		{
			*n = table[k].value;
			// check for + or - an integer
			ptr += incr;
			if (eatwhite(ptr) == NULL) return TRUE; // nothing else on the line
			ptr = eatwhite(ptr);                    // move up to wahtever it is...
			if (*ptr == '-' || *ptr == '+')
			{
				if (getnum(ptr, &m)) *n += m;
			}
			return TRUE;
		}
	}
	return FALSE;
}

void init_processor(processor* cpu, int codesize, int datasize)
{
	int i;
	for (i = 0; i < 64; i++)
	{
		cpu->rreg[i].val = cpu->wreg[i].val = 0;
		cpu->rreg[i].source = cpu->wreg[i].source = FROM_REGISTER;
	}
	cpu->PC = 0;
	cpu->codesize = codesize;
	cpu->datasize = datasize;
	cpu->code = NULL;
	cpu->data = NULL;
	cpu->status = RUNNING;
}

void init_pipeline(pipeline* pipe, int ADDS, int MULS, int DIVS)
{
	int i;
	pipe->branch = FALSE;
	pipe->destination = 0;
	pipe->ADD_LATENCY = ADDS;
	pipe->MUL_LATENCY = MULS;
	pipe->DIV_LATENCY = DIVS;

	pipe->if_id.active = FALSE;
	pipe->integer.active = FALSE;
	pipe->ex_mem.active = FALSE;
	pipe->mem_wb.active = FALSE;
	for (i = 0; i < ADDS; i++) pipe->a[i].active = FALSE;
	for (i = 0; i < MULS; i++) pipe->m[i].active = FALSE;
	pipe->div.active = FALSE;
	pipe->div.cycles = 0;
	pipe->active = TRUE;
	pipe->halting = FALSE;
	pipe->mem_wb.condition = TRUE;
	pipe->ex_mem.condition = TRUE;
}

static char const* conventionRegisterName[32] = {
  "0 ",
  "at",
  "v0",
  "v1",
  "a0",
  "a1",
  "a2",
  "a3",
  "t0",
  "t1",
  "t2",
  "t3",
  "t4",
  "t5",
  "t6",
  "t7",
  "s0",
  "s1",
  "s2",
  "s3",
  "s4",
  "s5",
  "s6",
  "s7",
  "t8",
  "t9",
  "k0",
  "k1",
  "gp",
  "sp",
  "fp",
  "ra"
};

char* registerName(int regnum, char* buffer, BOOL registerAsNumber) {
	// TODO buffer size 10 is fixed, should be passed as parameter
	if (registerAsNumber == TRUE) {
		sprintf_s(buffer, 10, "R%d=  ", regnum);
	}
	else {
		sprintf_s(buffer, 10, "$%s=  ", conventionRegisterName[regnum]);
	}

	return buffer;
}
