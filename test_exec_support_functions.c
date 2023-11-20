// Checks the Exec support functions: BeginIO, CreateExtIO, DeleteExtIO, CreatePort, DeletePort, CreateStdIO, DeleteStdIO, CreateTask, DeleteTask,
// NewList, LibCreatePool, LibAllocPooled, LibFreePooled, LibDeletePool
#include <exec/types.h>
#include <proto/exec.h>
#include <exec/tasks.h>
#include <proto/dos.h>
#include <devices/input.h>
#include <clib/alib_protos.h>

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

#define PUDDLE_SIZE 4096UL
#define THRES_SIZE 4096UL
#define MEM_SIZE 2048UL

static const UBYTE shortTaskName[] = "ShortTask";
static BOOL shortTaskFlag;
static struct List list;

// Function executed after CreateTask
static VOID fcntShortTask(VOID)
{
	shortTaskFlag = TRUE;
	Wait(0L); // wait until DeleteTask
}

WORD testExecSupportFunctions(void)
{
    struct timerequest *timeReq = NULL;
	struct IOStdReq *inputReq = NULL;
    struct MsgPort *timerPort = NULL,*inputPort = NULL;
	struct Task *aTask = NULL;
	APTR pool = NULL,mem = NULL;
	UBYTE mousePort = 0;
	WORD errCnt = 0;

	// Get time from timer.device
	timerPort  = CreatePort(NULL,0);

	if(timerPort  == NULL)
	{
		errCnt++;
		goto end;
	}

	timeReq = (struct timerequest *)CreateExtIO(timerPort,sizeof(struct timerequest));

	if(timeReq == NULL)
	{
		errCnt++;
		goto end;
	}

	if(OpenDevice("timer.device",UNIT_MICROHZ,(struct IORequest *)timeReq,0) != 0)
	{
		errCnt++;
		goto end;
	}

	timeReq->tr_node.io_Command = TR_GETSYSTIME;
	timeReq->tr_node.io_Flags = 0;
	timeReq->tr_time.tv_secs = 0L;
	timeReq->tr_time.tv_micro = 0L;
	BeginIO((struct IORequest *)timeReq);
	Wait(1L << timerPort->mp_SigBit);
	WaitIO((struct IORequest *)timeReq);

#if OPTION_TRACE
	Printf("seconds : %lu, microseconds : %lu\n",timeReq->tr_time.tv_secs,timeReq->tr_time.tv_micro);
#endif

	if(timeReq->tr_time.tv_secs == 0L || timeReq->tr_time.tv_micro == 0L)
		errCnt++;

    CloseDevice((struct IORequest *)timeReq);

end:
	if(timeReq != NULL)
		DeleteExtIO((struct IORequest *)timeReq);

	if(timerPort != NULL)
		DeletePort(timerPort);

	// Set the mouse port data from input.device
	inputPort  = CreatePort(NULL,0);

	if(inputPort  == NULL)
	{
		errCnt++;
		goto end2;
	}
	
	inputReq = CreateStdIO(inputPort);

	if(inputReq == NULL)
	{
		errCnt++;
		goto end2;
	}

	if(OpenDevice("input.device",0,(struct IORequest *)inputReq,0) != 0)
	{
		errCnt++;
		goto end2;
	}

	inputReq->io_Command = IND_SETMPORT;
	inputReq->io_Length = 1L;
	inputReq->io_Data = (APTR)&mousePort;
	DoIO((struct IORequest *)inputReq);

#if OPTION_TRACE
	Printf("input.device io_Error = %u\n",inputReq->io_Error);
#endif

	if(inputReq->io_Error != 0)
		errCnt++;

	 CloseDevice((struct IORequest *)inputReq);

end2:
	if(inputReq != NULL)
		DeleteStdIO((struct IOStdReq *)inputReq);

	if(inputPort != NULL)
		DeletePort(inputPort);

	// Create a task and delete it
	shortTaskFlag = FALSE;

	aTask = CreateTask(shortTaskName,0L,fcntShortTask,4096L);

	if(aTask == NULL)
	{
		errCnt++;
		goto end3;
	}

	Delay(50); // 1s is enough for the task to start

	if(shortTaskFlag == FALSE)
		errCnt++;

	// Initialize a list
	NewList(&list);

	if(list.lh_Head != (struct Node *)&list.lh_Tail || list.lh_Tail != NULL || list.lh_TailPred != (struct Node *)&list.lh_Head)
		errCnt++;

end3:
	if(aTask != NULL)
		DeleteTask(aTask);

	// Allocate memory with pools
	pool = LibCreatePool(MEMF_CLEAR,PUDDLE_SIZE,THRES_SIZE);

	if(pool == NULL)
	{
		Printf("ERROR1\n");
		errCnt++;
		goto end4;
	}

	mem = LibAllocPooled(pool,MEM_SIZE);

	if(mem == NULL)
	{
		Printf("ERROR2\n");
		errCnt++;
		goto end4;
	}

end4:
	if(mem != NULL)
		LibFreePooled(pool,mem,MEM_SIZE);

	if(pool != NULL)
		LibDeletePool(pool);

	return(errCnt);
}