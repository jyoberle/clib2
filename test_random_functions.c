// Checks the random functions: FastRand, RangeRand
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <clib/alib_protos.h>

#define FAST_RAND_SEED 0x12345678
#define RANGE_RAND_MAX_VALUE 0x5555

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

WORD testRandomFunctions(void)
{
	ULONG fastRandResult = 0L;
	UWORD rangeRandResult = 0;
	WORD errCnt = 0;

	fastRandResult = FastRand(FAST_RAND_SEED);
	rangeRandResult = RangeRand(RANGE_RAND_MAX_VALUE);

#if OPTION_TRACE
	Printf("fastRand: %lu\n",fastRandResult);
	Printf("rangeRand: %lu\n",rangeRandResult);
#endif

	if(fastRandResult == 0)
		errCnt++;

	if(rangeRandResult == 0 || rangeRandResult > RANGE_RAND_MAX_VALUE)
		errCnt++;

	return(errCnt);
}