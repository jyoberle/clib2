// Checks the timer support functions: TimeDelay, DoTimer
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <devices/timer.h>
#include <clib/alib_protos.h>

extern struct DosLibrary *DOSBase;

WORD testTimerSupportFunctions(void)
{
	struct timeval tv;
	WORD errCnt = 0;

	TimeDelay(0,1L,0L);

	tv.tv_secs = 0L;
	tv.tv_micro = 0L;

	DoTimer(&tv,0,TR_GETSYSTIME);

#if OPTION_TRACE
	Printf("seconds : %lu, microseconds : %lu\n",tv.tv_secs,tv.tv_micro);
#endif

	if(tv.tv_secs == 0L || tv.tv_micro == 0L)
		errCnt++;

	return(errCnt);
}