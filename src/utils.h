#ifndef UTILS
#define UTILS

#include "mytypes.h"

extern int bits(int);
extern int sprintnbits(char *,WORD32,int);
extern WORD32 pack32(BYTE*);
extern void unpack32(WORD32,BYTE *);
extern SIGNED16 pack16(BYTE*);
extern void unpack16(SIGNED16,BYTE *);
extern WORD64 pack(BYTE*);
extern void unpack(WORD64,BYTE *);
extern void sprintword32(char *,WORD32);
extern void sprintdouble(char *,double);
extern void sprintword(char *,WORD64);
extern BOOL in_range(WORD32,WORD32);
extern BOOL delimiter(int);
extern int compare(char*,char *);
extern char *skip(char *,char);
extern char *skipover(char *,char);
extern int is_symbol(char *);
extern char *eatwhite(char *);
extern int fgetreg(char *&);
extern int alignment(int,int);
extern int getreg(char *&);
extern WORD64 strtoint64(char *,char **,int);
extern BOOL getdouble(char *&ptr,double *);
extern BOOL getfullnum(char *&,WORD64 *);
extern BOOL getnum(char *&,WORD32 *);
extern BOOL getsym(symbol_table *,int,char *&,WORD32 *);
extern void init_processor(processor *,int,int);
extern void init_pipeline(pipeline *,int,int,int);
extern int clock_tick(pipeline *,processor *,BOOL,BOOL,BOOL,RESULT *);
extern char *registerName(int regnum, char *buffer, BOOL registerAsNumber);
#endif