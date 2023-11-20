#include <proto/utility.h>

extern struct UtilityBase *UtilityBase;

ULONG CallHookA(struct Hook *hook,Object *obj,APTR msg)
{
    return(CallHookPkt(hook,obj,msg));
}