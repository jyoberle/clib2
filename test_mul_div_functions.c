// Checks the mul/div functions
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <clib/alib_protos.h>
#include <string.h>

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;
extern LONG __modsi3(LONG dividend,LONG divisor);
extern LONG __mulsi3(LONG Arg1,LONG Arg2);
extern ULONG __udivsi3(ULONG dividend,ULONG divisor);
extern ULONG __umodsi3(ULONG dividend,ULONG divisor);

#define VAL1 -100025L
#define VAL2 257L
#define REMAINDER1 -52L
#define MUL2 -25706425L
#define VAL3 568970L
#define VAL4 5686L
#define QUOTIENT3 100L
#define REMAINDER4 370L

WORD testMulDivFunctions(void)
{
	WORD errCnt = 0;
	LONG sres;
    ULONG res;

    sres = __modsi3(VAL1,VAL2);

#if OPTION_TRACE
    Printf("sres = %ld\n",sres);
#endif

    if(sres != REMAINDER1)
    {
        errCnt++;
    }

    sres = __mulsi3(VAL1,VAL2);

#if OPTION_TRACE
    Printf("sres = %ld\n",sres);
#endif

    if(sres != MUL2)
    {
        errCnt++;
    }

    res = __udivsi3(VAL3,VAL4);

#if OPTION_TRACE
    Printf("res = %ld\n",res);
#endif

    if(res != QUOTIENT3)
    {
        errCnt++;
    }

    res = __umodsi3(VAL3,VAL4);

#if OPTION_TRACE
    Printf("res = %ld\n",res);
#endif

    if(res != REMAINDER4)
    {
        errCnt++;
    }

	return(errCnt);
}