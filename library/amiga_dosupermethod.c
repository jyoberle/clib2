#include "l_alib.h"
#include <proto/exec.h>
#include <clib/alib_protos.h>
#include <stdarg.h>

ULONG DoSuperMethod(Class *cl,Object *obj,ULONG method_id, ...)
{
    ULONG result = 0L;
	va_list param;
	ULONG *msg;
	int i;

    if(cl != NULL && obj != NULL)
    {
		if ((msg = AllocVec(ARG_CNT*sizeof(ULONG),MEMF_ANY)))
		{
			msg[0] = method_id;
			va_start(param,method_id);
			
			for(int i=1;i<ARG_CNT;i++) {
				msg[i] = (ULONG)va_arg(param,ULONG);
			}

			va_end(param);

			result = DoSuperMethodA(cl,obj,(Msg)msg);
			FreeVec(msg);
		}
    }

    return(result);
}