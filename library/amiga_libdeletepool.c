#include <proto/exec.h>
#include "l_alib.h"

VOID LibDeletePool(APTR poolHeader)
{
    struct ExecBase *SysBase = *((struct ExecBase**)4UL);

    if(((struct Library *)SysBase)->lib_Version < MIN_EXEC_VERSION)
        return;

    DeletePool(poolHeader);
}