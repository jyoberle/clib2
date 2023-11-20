#include <proto/exec.h>
#include "l_alib.h"

VOID LibFreePooled(APTR poolHeader,APTR memory,ULONG memSize)
{
    struct ExecBase *SysBase = *((struct ExecBase**)4UL);

    if(((struct Library *)SysBase)->lib_Version < MIN_EXEC_VERSION)
        return;

    FreePooled(poolHeader,memory,memSize);
}