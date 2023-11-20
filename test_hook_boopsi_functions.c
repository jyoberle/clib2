// Checks the hook and boopsi support functions: CallHookA, CallHook, DoMethodA, DoMethod, DoSuperMethodA, DoSuperMethod, CoerceMethodA, CoerceMethod, HookEntry, SetSuperAttrs
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <clib/alib_protos.h>
#include <string.h>

extern struct ExecBase *SysBase;
extern struct IntuitionBase *IntuitionBase;
extern struct DosLibrary *DOSBase;
extern struct UtilityBase *UtilityBase;

#define ALPHA_TAG (TAG_USER + 1)
#define BETA_TAG (TAG_USER + 2)
#define GAMMA_TAG (TAG_USER + 3)
#define DELTA_TAG (TAG_USER + 4)
#define OBJECT_VALUE_1 256
#define OBJECT_VALUE_2 1000
#define OBJECT_VALUE_3 64
#define OBJECT_VALUE_4 128
#define OBJECT_VALUE_5 2000
#define OBJECT_VALUE_6 3000

struct Class1Data {
	ULONG alpha;
	ULONG beta;
};

struct Class2Data {
	ULONG gamma;
	ULONG delta;
};

static ULONG dispatcherClass1(Class *cl,Object *o,Msg msg)
{
	struct Class1Data *data;
	struct TagItem *tstate,*ti;
	APTR retval = NULL;

	switch(msg->MethodID)
    {
		case OM_NEW:
			if(retval = (APTR)DoSuperMethodA(cl,o,msg))
			{
				data = INST_DATA(cl,retval);
				data->alpha = GetTagData(ALPHA_TAG,0L,((struct opSet *)msg)->ops_AttrList);
				data->beta = GetTagData(BETA_TAG,0L,((struct opSet *)msg)->ops_AttrList);
			}
			break;

        case OM_SET:
        case OM_UPDATE:
            data = INST_DATA(cl,o);
			DoSuperMethodA(cl,o,msg);
			ti = ((struct opSet *)msg)->ops_AttrList;
			tstate = ti;

			while(ti = NextTagItem(&tstate))
			{
				switch (ti->ti_Tag)
				{
					case ALPHA_TAG:
						data->alpha = ti->ti_Data;
						break;

					case BETA_TAG:
						data->beta = ti->ti_Data;
						break;
				}
			}
			break;

		case OM_GET:
			data = INST_DATA(cl,o);

			switch(((struct opGet *)msg)->opg_AttrID)
			{
				case ALPHA_TAG:
					*(((struct opGet *)msg)->opg_Storage) = data->alpha;
					retval = (Object *)TRUE;
					break;
				
				case BETA_TAG:
					*(((struct opGet *)msg)->opg_Storage) = data->beta;
					retval = (Object *)TRUE;
					break;

				default:
					retval = (APTR)DoSuperMethodA(cl,o,msg);
					break;

			}
			break;

		default: // includes OM_DISPOSE
			retval = (APTR)DoSuperMethodA(cl,o,msg);
			break;			
	}

	return((ULONG)retval);
}

static ULONG dispatcherClass2(Class *cl,Object *o,Msg msg)
{
	struct Class2Data *data;
	struct TagItem *tstate,*ti;
	APTR retval = NULL;

	switch (msg->MethodID)
    {
		case OM_NEW:
			if(retval = (APTR)DoSuperMethodA(cl,o,msg))
			{
				data = INST_DATA(cl,retval);
				data->gamma = GetTagData(GAMMA_TAG,0L,((struct opSet *)msg)->ops_AttrList);
				data->delta = GetTagData(DELTA_TAG,0L,((struct opSet *)msg)->ops_AttrList);
			}
			break;

        case OM_SET:
        case OM_UPDATE:
            data = INST_DATA(cl,o);
			DoSuperMethodA(cl,o,msg);
			ti = ((struct opSet *)msg)->ops_AttrList;
			tstate = ti;

			while(ti = NextTagItem(&tstate))
			{
				switch (ti->ti_Tag)
				{
					case GAMMA_TAG:
						data->gamma = ti->ti_Data;
						break;

					case DELTA_TAG:
						data->delta = ti->ti_Data;
						break;
				}
			}
			break;

		case OM_GET:
			data = INST_DATA(cl,o);
			switch(((struct opGet *)msg)->opg_AttrID)
			{
				case GAMMA_TAG:
					*(((struct opGet *)msg)->opg_Storage) = data->gamma;
					retval = (Object *)TRUE;
					break;
				
				case DELTA_TAG:
					*(((struct opGet *)msg)->opg_Storage) = data->delta;
					retval = (Object *)TRUE;
					break;
				default:
					retval = (APTR)DoSuperMethodA(cl,o,msg);
					break;

			}
			break;

		default: // includes OM_DISPOSE
			retval = (APTR)DoSuperMethodA(cl,o,msg);
			break;			
	}

	return((ULONG)retval);
}

WORD testHookBoopsiFunctions(void)
{
	Class *cl1 = NULL,*cl2 = NULL;
	Object *obj = NULL;
	ULONG val;
	WORD errCnt = 0;
	struct opGet getVal;

	cl1 = MakeClass(NULL,"rootclass",NULL,sizeof(struct Class1Data),0L);

	if(cl1 == NULL)
	{
		errCnt++;
		goto end;
	}

	cl1->cl_Dispatcher.h_Entry = HookEntry;
	cl1->cl_Dispatcher.h_SubEntry = dispatcherClass1;

	cl2 = MakeClass(NULL,NULL,cl1,sizeof(struct Class2Data),0L);

	if(cl2 == NULL)
	{
		errCnt++;
		goto end;
	}

	cl2->cl_Dispatcher.h_Entry = HookEntry;
	cl2->cl_Dispatcher.h_SubEntry = dispatcherClass2;

	// Create the object
	obj = NewObject(cl2,NULL,ALPHA_TAG,OBJECT_VALUE_1,BETA_TAG,OBJECT_VALUE_2,GAMMA_TAG,OBJECT_VALUE_3,DELTA_TAG,OBJECT_VALUE_4,TAG_END);

	if(obj == NULL)
	{
		errCnt++;
		goto end;
	}	

	// Get a value from the super class
	DoMethod(obj,OM_GET,ALPHA_TAG,&val);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_1)
	{
		errCnt++;
	}

	// Get a value from the class
	DoMethod(obj,OM_GET,GAMMA_TAG,&val);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_3)
	{
		errCnt++;
	}	

	// Get the other value from the super class
	getVal.MethodID = OM_GET;
	getVal.opg_AttrID = BETA_TAG;
	getVal.opg_Storage = &val;

	DoMethodA(obj,(Msg)&getVal);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_2)
	{
		errCnt++;
	}

	// Get the other value from the class
	getVal.MethodID = OM_GET;
	getVal.opg_AttrID = DELTA_TAG;
	getVal.opg_Storage = &val;

	DoMethodA(obj,(Msg)&getVal);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_4)
	{
		errCnt++;
	}

	// Get a value directly from the super class
	DoSuperMethod(cl2,obj,OM_GET,ALPHA_TAG,&val);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_1)
	{
		errCnt++;
	}

	// Get the other value directly from the super class
	getVal.MethodID = OM_GET;
	getVal.opg_AttrID = BETA_TAG;
	getVal.opg_Storage = &val;

	DoSuperMethodA(cl2,obj,(Msg)&getVal);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_2)
	{
		errCnt++;
	}

	// Get a value directly from the super class
	CoerceMethod(cl1,obj,OM_GET,ALPHA_TAG,&val);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_1)
	{
		errCnt++;
	}

	// Get the other value directly from the super class
	getVal.MethodID = OM_GET;
	getVal.opg_AttrID = BETA_TAG;
	getVal.opg_Storage = &val;

	CoerceMethodA(cl1,obj,(Msg)&getVal);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_2)
	{
		errCnt++;
	}

	// Set the values directly in the super class
	SetSuperAttrs(cl2,obj,ALPHA_TAG,OBJECT_VALUE_5,BETA_TAG,OBJECT_VALUE_6,TAG_END);
	CoerceMethod(cl1,obj,OM_GET,ALPHA_TAG,&val); // we read the value back directly from the super class

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_5)
	{
		errCnt++;
	}

	CoerceMethod(cl1,obj,OM_GET,BETA_TAG,&val);

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_6)
	{
		errCnt++;
	}

	CallHook(&cl1->cl_Dispatcher,obj,OM_GET,ALPHA_TAG,&val); // CoerceMethod

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_5)
	{
		errCnt++;
	}

	getVal.MethodID = OM_GET;
	getVal.opg_AttrID = BETA_TAG;
	getVal.opg_Storage = &val;

	CallHookA(&cl1->cl_Dispatcher,obj,&getVal); // CoerceMethod

#if OPTION_TRACE
	Printf("val = %lu\n",val);
#endif

	if(val != OBJECT_VALUE_6)
	{
		errCnt++;
	}	

end:
	if(obj != NULL)
		DisposeObject(obj);

	if(cl2 != NULL)
		FreeClass(cl2);

	if(cl1 != NULL)
		FreeClass(cl1);

	return(errCnt);
}
