// Checks the Arexx functions: CheckRexxMsg, GetRexxVar, SetRexxVar
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/rexxsyslib.h>
#include <utility/tagitem.h>
#include <clib/alib_protos.h>
#include <stdlib_constructor.h>

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

extern	void __ctor_rexxvars_init(void);
extern	void __dtor_rexxvars_exit(void);

WORD testArexxFunctions(void)
{
	struct MsgPort *rexxPort = NULL;
	struct RexxMsg *rexxMsg;
	ULONG rexxsigflag,sigrcvd;
	BOOL loop = TRUE;
	struct TagItem stags[1];
	BPTR rexxFile;
	LONG error;
	unsigned char *value;
	WORD errCnt = 0;

	__ctor_rexxvars_init();

	rexxPort  = CreatePort("VARTEST",0);

	if(rexxPort == NULL)
	{
		errCnt++;
		goto end;
	}

    stags[0].ti_Tag = TAG_END;
	error = SystemTagList("df0:System/rexxmast > NIL:",stags);

	if(error != 0L)
	{
		errCnt++;
		goto end;
	}

	rexxFile = Open("RAM:testrexx",MODE_NEWFILE);

	if(rexxFile == NULL)
	{
		errCnt++;
		goto end;		
	}

	VFPrintf(rexxFile,"/* Testing the variable interface */\nA = 1\nADDRESS 'VARTEST' test\nsay A\n",NULL); // no argv
	Close(rexxFile);

	error = SystemTagList("run > NIL: df0:rexxc/rx RAM:testrexx > NIL:",stags);

	if(error != 0L)
	{
		errCnt++;
		goto end;
	}

	rexxsigflag = 1L << rexxPort->mp_SigBit;

	while(loop)
	{
		sigrcvd = Wait(SIGBREAKF_CTRL_C | rexxsigflag);

		while((rexxMsg = (struct RexxMsg *)GetMsg(rexxPort)))
		{
			if(CheckRexxMsg(rexxMsg))
			{
    			if(SetRexxVar(rexxMsg,"A","2",1))
    			{
					errCnt++;
				}
                else
                {
					if(GetRexxVar(rexxMsg,"A",&value))
					{
						errCnt++;
					} 
					else 
					{
						if(value[0] != '2')
						{
							errCnt++;
						}
					}
    			}

				loop = FALSE;
			}
			else
			{
				errCnt++;
			}

			ReplyMsg((struct Message *)rexxMsg);
		}

		if(sigrcvd & SIGBREAKF_CTRL_C)
		{
			errCnt++;
			loop = FALSE;
		}
	}

end:
	if(rexxPort != NULL)
	{
		while((rexxMsg = (struct RexxMsg *)GetMsg(rexxPort)))
			ReplyMsg((struct Message *)rexxMsg);

		DeletePort(rexxPort);
	}

	__dtor_rexxvars_exit();

	return(errCnt);
}
