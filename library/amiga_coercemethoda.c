#include <clib/alib_protos.h>

ULONG CoerceMethodA(Class *cl,Object * obj,Msg msg)
{
    ULONG result = 0L;

    if(cl != NULL && obj != NULL)
		result = CallHookA(&cl->cl_Dispatcher,obj,msg);
	
	return(result);
}