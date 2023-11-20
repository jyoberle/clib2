// Checks the graphic support functions
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <clib/alib_protos.h>

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

#define VBLANK_SERVER_VAR_VALUE_1 0xABCD1234
#define VBLANK_SERVER_VAR_VALUE_2 0xABCD5555
#define WAIT_BEAM_POS 0x20

static ULONG VBlankServerVar;
static UBYTE AddTOFIntName[] = "AddTOFIntName";

// Function called as part of the vertical-blanking interval interrupt server
static __attribute__ ((noinline)) LONG VBlankServerFunction(APTR arg)
{
	ULONG *ptr = (ULONG *)arg;
	*ptr = VBLANK_SERVER_VAR_VALUE_1;
	return(0L);
}

// Checks the graphic support functions: AddTOF, RemTOF, waitbeam
WORD testGraphicSupportFunctions(void)
{
	struct Isrvstr i;
	WORD errCnt = 0;

	i.is_Node.ln_Succ = NULL;
	i.is_Node.ln_Pred = NULL;
	i.is_Node.ln_Type = NT_INTERRUPT;
	i.is_Node.ln_Pri = -127;
	i.is_Node.ln_Name = AddTOFIntName;

	VBlankServerVar = 0L;
	AddTOF(&i,VBlankServerFunction,&VBlankServerVar);
	Delay(50);

#if OPTION_TRACE
	Printf("VBlankServerVar = %lu\n",VBlankServerVar);
#endif
	
	if(VBlankServerVar != VBLANK_SERVER_VAR_VALUE_1)
		errCnt++;

	RemTOF(&i);
	Delay(50);
	VBlankServerVar = VBLANK_SERVER_VAR_VALUE_2;

#if OPTION_TRACE
	Printf("VBlankServerVar = %lu\n",VBlankServerVar);
#endif

	if(VBlankServerVar != VBLANK_SERVER_VAR_VALUE_2)
		errCnt++;

	waitbeam(WAIT_BEAM_POS);

	return(errCnt);
}