#include <proto/exec.h>
#include "l_alib.h"

APTR LibAllocPooled(APTR poolHeader,ULONG memSize)
{
    struct ExecBase *SysBase = *((struct ExecBase**)4UL);

    if(((struct Library *)SysBase)->lib_Version < MIN_EXEC_VERSION)
        return(NULL);

    return(AllocPooled(poolHeader,memSize));
}