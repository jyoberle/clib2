#include <proto/utility.h>

ULONG DoMethodA(Object * obj,Msg msg)
{
	ULONG result = 0;

	if(obj != NULL)
	{
		Class *cl = OCLASS(obj);

		if(cl != NULL) {
			result = CallHookPkt(&cl->cl_Dispatcher,obj,msg);
		}
	}

	return(result);
}
