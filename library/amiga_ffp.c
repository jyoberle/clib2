// VSCode's IntelliSense doesn't know about 68000 registers, so suppress warnings
#ifndef __INTELLISENSE__
    #define ASM __asm
#else
    #define ASM(...)
#endif

#include <proto/exec.h>
#define FLOAT ULONG

FLOAT afp(CONST_STRPTR ffpstr)
{
	register volatile const char* _a0 ASM("a0") = ffpstr;
	register volatile       FLOAT _d7 ASM("d7"); // return value
	register volatile       ULONG _d3 ASM("d3");
	register volatile       ULONG _d4 ASM("d4");
	register volatile       ULONG _d5 ASM("d5");
	register volatile       ULONG _d6 ASM("d6");
	__asm volatile (
		"jsr FFPAFP\n"
	: "=r"(_d7),"=r"(_a0),"=r"(_d3),"=r"(_d4),"=r"(_d5),"=r"(_d6)
	: "r"(_a0)
	: "fp0", "fp1", "cc", "memory");
	return(_d7);
}

VOID arnd(LONG place,LONG exp,STRPTR string) // no ending '\0' will be appended to the string
{
	register volatile const char* _a0 ASM("a0") = string;
	register volatile       LONG  _d6 ASM("d6") = place;
	register volatile       LONG  _d7 ASM("d7") = exp;
	register volatile       LONG  _d0 ASM("d0");
	register volatile       LONG  _a1 ASM("a1");
	__asm volatile (
		"adda.l #14,%%a0\n" // point to end of string
		"move.l %%sp,%%a1\n" // a0 may not be word aligned
		"moveq #13,%%d0\n" // 14 bytes for string
		"%=: move.b -(%%a0),-(%%a1)\n" // string on the stack
		"dbra %%d0,%=b\n"
		"move.l %%a1,%%sp\n"
		"jsr FFPARND\n"
		"moveq #13,%%d0\n"
		"%=: move.b (%%a1)+,(%%a0)+\n" // string back
		"dbra %%d0,%=b\n"
		"move.l %%a1,%%sp\n"
	: "=r"(_d0),"=r"(_a0),"=r"(_a1)
	: "r"(_a0),"r"(_d6),"r"(_d7)
	: "fp0", "fp1", "cc", "memory");	
}

FLOAT dbf(ULONG exp,ULONG mant)
{
	register volatile ULONG _d6 ASM("d6") = exp;
	register volatile ULONG _d7 ASM("d7") = mant; // return value
	register volatile ULONG _d3 ASM("d3");
	register volatile ULONG _d4 ASM("d4");
	register volatile ULONG _d5 ASM("d5");
	__asm volatile (
		"jsr FFPDBF\n"
	: "=r"(_d7),"=r"(_d3),"=r"(_d4),"=r"(_d5),"=r"(_d6)
	: "r"(_d6),"r"(_d7)
	: "fp0", "fp1", "cc", "memory");
	return(_d7);
}

LONG fpa(FLOAT fnum,BYTE *string) // no ending '\0' will be appended to the string
{
	register volatile BYTE* _a0 ASM("a0") = string;
	register volatile FLOAT _d7 ASM("d7") = fnum; // also contains the result
	register volatile ULONG _d0 ASM("d0");
	register volatile FLOAT _a1 ASM("a1");
	register volatile FLOAT _a6 ASM("a6");
	__asm volatile (
		"move.l 4,%%a6\n" // we will call GetCC in FFPFPA
		"jsr FFPFPA\n"
		"move.l %%sp,%%a1\n" // a0 may not be word aligned
		"moveq #13,%%d0\n" // 14 bytes for string
		"%=: move.b (%%a1)+,(%%a0)+\n"
		"dbra %%d0,%=b\n"
		"move.l %%a1,%%sp\n"
	: "=r"(_d7),"=r"(_a0),"=r"(_d0),"=r"(_a1),"=r"(_a6)
	: "r"(_d7),"r"(_a0)
	: "fp0", "fp1", "cc", "memory");
	return(_d7);
}

VOID fpbcd(FLOAT fnum,BYTE *string)
{
	register volatile BYTE* _a0 ASM("a0") = string;
	register volatile FLOAT _d7 ASM("d7") = fnum;
	register volatile FLOAT _d0 ASM("d0");
	register volatile FLOAT _a1 ASM("a1");
	register volatile FLOAT _a6 ASM("a6");
	__asm volatile (
		"move.l 4,%%a6\n" // we will call GetCC in ffpfpbcd
		"jsr ffpfpbcd\n"
		"move.l %%sp,%%a1\n" // a0 may not be word aligned
		"moveq #7,%%d0\n" // 8 bytes for output
		"%=: move.b (%%a1)+,(%%a0)+\n"
		"dbra %%d0,%=b\n"
		"move.l %%a1,%%sp\n"
	: "=r"(_d0),"=r"(_a0),"=r"(_a1),"=r"(_a6)
	: "r"(_d7),"r"(_a0)
	: "fp0", "fp1", "cc", "memory");
}
