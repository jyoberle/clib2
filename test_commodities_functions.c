// Checks the commodities functions: ArgArrayDone, ArgArrayInit, ArgInt, ArgString, HotKey, InvertString, FreeIEvents
// Inspired by https://wiki.amigaos.net/wiki/Commodities_Exchange_Library
#include <exec/types.h>
#include <proto/exec.h>
#include <proto/commodities.h>
#include <proto/dos.h>
#include <proto/commodities.h>
#include <proto/utility.h>
#include <clib/alib_protos.h>

extern struct ExecBase *SysBase;
extern struct IntuitionBase *IntuitionBase;
extern struct DosLibrary *DOSBase;

#define TOOL_TYPES_CNT 3
#define EVT_HOTKEY_1 1L
#define EVT_HOTKEY_2 1L

static CONST_STRPTR toolTypes[] = {"command name","TOOL_LEVEL=5","TOOL_NAME=amiga","TOOL_VERSION=1.0.1"};
static const char hotkey_str_1[] = "ctrl alt a";
static const char hotkey_str_2[] = "shift alt a";
struct NewBroker newbroker = {
    NB_VERSION,
    "Amiga HotKey",           /* string to identify this broker */
    "A Simple HotKey",
    "A simple hot key commodity",
    NBU_UNIQUE | NBU_NOTIFY,    /* Don't want any new commodities starting with this name. */
    0, 0, 0, 0                  /* If someone tries it, let me know */
};
CONST_STRPTR shift_alt_a = "\n<shift alt a>";

WORD testCommoditiesFunctions(void)
{
	STRPTR *ttypes;
	LONG toolLevel = 0;
	char *toolStr;
	struct MsgPort *cxPort = NULL;
	struct Message *cxMsg;
	CxObj *filter1 = NULL, *filter2 = NULL;
	CxObj *broker = NULL;
	CxMsg *msg = NULL;
	ULONG cxsigflag;
	ULONG sigrcvd;
	BOOL loop = TRUE;
	ULONG msgid,msgtype;
	struct InputEvent *ie = NULL;
	WORD errCnt = 0;

	ttypes = ArgArrayInit(TOOL_TYPES_CNT,toolTypes);
	toolLevel = (LONG)ArgInt((CONST_STRPTR *)ttypes,"TOOL_LEVEL",10);

#if OPTION_TRACE
	Printf("toolLevel = %lu\n",toolLevel);
#endif

	if(toolLevel != 5L)
	{
		errCnt++;
	}

	toolStr = ArgString((CONST_STRPTR *)ttypes, "TOOL_NAME", "no name");

#if OPTION_TRACE
	Printf("toolStr = %ls\n",toolStr);
#endif

	if(Stricmp(toolStr,"amiga"))
	{
		errCnt++;
	}

	ArgArrayDone();

	cxPort  = CreatePort(NULL,0);

	if(cxPort == NULL)
	{
		errCnt++;
		goto end;
	}

	newbroker.nb_Port = cxPort;
	cxsigflag = 1L << cxPort->mp_SigBit;
	newbroker.nb_Pri = 0;

	broker = CxBroker(&newbroker,NULL);

	if(broker == NULL)
	{
		errCnt++;
		goto end;		
	}

	filter1 = HotKey(hotkey_str_1,cxPort,EVT_HOTKEY_1);

	if(filter1 == NULL)
	{
		errCnt++;
		goto end;
	}

	filter2 = HotKey(hotkey_str_2,cxPort,EVT_HOTKEY_2);

	if(filter2 == NULL)
	{
		errCnt++;
		goto end;
	}

	ie = InvertString(shift_alt_a, NULL);

	if(ie == NULL)
	{
		errCnt++;
		goto end;		
	}

	AttachCxObj(broker,filter1);
	AttachCxObj(broker,filter2);
	ActivateCxObj(broker,1L);

	Printf("Please press %ls...\n",(long unsigned int)hotkey_str_1);

	while(loop)
	{
		sigrcvd = Wait(SIGBREAKF_CTRL_C | cxsigflag);

		while((msg = (CxMsg *)GetMsg(cxPort)))
		{
			msgid = CxMsgID(msg);
			msgtype = CxMsgType(msg);
			ReplyMsg((struct Message *)msg);

			if(msgtype == CXM_IEVENT && msgid == EVT_HOTKEY_1)
			{
				AddIEvents(ie);
			}
			
			if(msgtype == CXM_IEVENT && msgid == EVT_HOTKEY_2)
			{
				loop = FALSE;
			}
		}

		if(sigrcvd & SIGBREAKF_CTRL_C)
		{
			errCnt++;
			loop = FALSE;
		}
	}

	ActivateCxObj(filter1,FALSE);
	ActivateCxObj(filter2,FALSE);
	FreeIEvents(ie);

end:
	if(broker != NULL)
	{
		DeleteCxObjAll(broker);
	}
	else
	{
		if(filter1 != NULL)
			DeleteCxObj(filter1);

		if(filter2 != NULL)
			DeleteCxObj(filter2);
	}

	if(cxPort != NULL)
	{
		while(msg = (CxMsg *)GetMsg(cxPort))
			ReplyMsg((struct Message *)msg);

		DeletePort(cxPort);
	}

	return(errCnt);
}