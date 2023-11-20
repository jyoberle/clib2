#include "l_alib.h"
#include <proto/exec.h>
#include <clib/alib_protos.h>
#include <stdarg.h>

ULONG CallHook(struct Hook *hook,Object *obj,... )
{
    ULONG result = 0L;
	va_list param;
	APTR *msg;
	int i;

	if(obj != NULL)
	{
		if ((msg = AllocVec(ARG_CNT*sizeof(ULONG),MEMF_ANY)))
		{
			va_start(param,obj);
			
			for(int i=0;i<ARG_CNT;i++) {
				msg[i] = (APTR)va_arg(param,APTR);
			}

			va_end(param);

			result = CallHookA(hook,obj,(APTR)msg);
			FreeVec(msg);
		}
	}

	return(result);
}