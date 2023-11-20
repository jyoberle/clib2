#include <clib/alib_protos.h>

ULONG DoSuperMethodA(Class * cl,Object * obj,Msg msg)
{
    ULONG result = 0L;

    if(cl != NULL && obj != NULL)
		result = CallHookA(&cl->cl_Super->cl_Dispatcher,obj,msg);

    return(result);
}