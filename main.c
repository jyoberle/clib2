// VSCode's IntelliSense doesn't know about 68000 registers, so suppress warnings
#ifndef __INTELLISENSE__
    #define ASM __asm
#else
    #define ASM(...)
#endif

#include <exec/types.h>
#include <proto/exec.h>
#include <exec/tasks.h>
#include <proto/intuition.h>
#include <proto/commodities.h>
#include <proto/dos.h>
#include <proto/rexxsyslib.h>
#include <proto/utility.h>
#include <proto/icon.h>
#include <graphics/gfxbase.h>
#include <devices/timer.h>
#include <devices/input.h>
#include <dos/dostags.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib_headers.h>

extern int __ctor_stdlib_memory_init(void);
extern int __ctor_stdlib_program_name_init(void);
extern int __ctor_stdio_init(void);
extern int __ctor_stdio_file_init(void);
extern int __ctor_math_init(void);
extern int __ctor_socket_init(void);
extern int __ctor_arg_init(void);
extern int __ctor_rexxvars_init(void);
extern int __ctor_dirent_init(void);
extern int __ctor_locale_init(void);
extern int __ctor_clock_init(void);
extern int __ctor_unistd_init(void);
extern int __ctor_timer_init(void);
extern int __ctor_usergroup_init(void);

extern int __dtor_usergroup_exit();
extern int __dtor_timer_exit();
extern int __dtor_unistd_exit();
extern int __dtor_locale_exit();
extern int __dtor_dirent_exit();
extern int __dtor_rexxvars_exit();
extern int __dtor_socket_exit();
extern int __dtor_math_exit();
extern int __dtor_workbench_exit();
extern int __dtor_stdio_exit();
extern int __dtor_stdlib_program_name_exit();
extern int __dtor_stdlib_memory_exit();
extern int __dtor___wildcard_expand_exit();
extern int __dtor_alloca_exit();
extern int __dtor___setenv_exit();
extern int __dtor___chdir_exit();

extern WORD testClibFunctions(void);
extern WORD testMathFunctions(void);
extern WORD testUnixlibFunctions(void);
extern WORD testNetlibFunctions(void);
extern WORD testDebuglibFunctions(void);
extern WORD testHookBoopsiFunctions(void);
extern WORD testFFP(void);
extern WORD testExecSupportFunctions(void);
extern WORD testRandomFunctions(void);
extern WORD testGraphicSupportFunctions(void);
extern WORD testTimerSupportFunctions(void);
extern WORD testCommoditiesFunctions(void);
extern WORD testArexxFunctions(void);
extern WORD testACryptFunction(void);
extern WORD testMulDivFunctions(void);

struct ExecBase *SysBase;
struct IntuitionBase *IntuitionBase = NULL;
struct DosLibrary *DOSBase = NULL;
struct UtilityBase *UtilityBase = NULL;
struct GfxBase *GfxBase = NULL;
struct Library *CxBase = NULL;
struct Library *IconBase = NULL;

// Calls all constructors
static VOID fcntCallCtor(void)
{	
	__ctor_stdlib_memory_init();
	__ctor_stdlib_program_name_init();
	__ctor_stdio_init();
	__ctor_stdio_file_init();
	__ctor_math_init();
	__ctor_socket_init();
	__ctor_arg_init();
	__ctor_rexxvars_init();
	__ctor_dirent_init();
	__ctor_locale_init();
	__ctor_clock_init();
	__ctor_unistd_init();
	__ctor_timer_init();
	__ctor_usergroup_init();
}

// Calls all destructors
static VOID fcntCallDtor(void)
{	
	__dtor___wildcard_expand_exit();
	__dtor_alloca_exit();
	__dtor___setenv_exit();
	__dtor___chdir_exit();
	__dtor_usergroup_exit();
	__dtor_timer_exit();
	__dtor_unistd_exit();
	__dtor_locale_exit();
	__dtor_dirent_exit();
	__dtor_rexxvars_exit();
	__dtor_socket_exit();
	__dtor_math_exit();
	__dtor_workbench_exit();
	__dtor_stdio_exit();
	__dtor_stdlib_program_name_exit();
	__dtor_stdlib_memory_exit();
}

// Does the clean up of all ressources
void cleanUP(void)
{
	if(IntuitionBase != NULL)
		CloseLibrary((struct Library *)IntuitionBase);

	if(DOSBase != NULL)
		CloseLibrary((struct Library *)DOSBase);

	if(UtilityBase != NULL)
		CloseLibrary((struct Library *)UtilityBase);

	if(GfxBase != NULL)
		CloseLibrary((struct Library *)GfxBase);

	if(CxBase != NULL)
		CloseLibrary(CxBase);

	if(IconBase != NULL)
		CloseLibrary(IconBase);	
}

// Entry of our code
int main(int argc, char **argv) 
{
	SysBase = *((struct ExecBase**)4UL);
	struct Process *proc;
	struct WBStartup *wbmsg = NULL;
	WORD i;

	// We check if our program has been started by the workbench or by the shell
	if (argc == 0) {
		// We have been launched by workbench: we wait for the startup message
		proc = (struct Process *)FindTask(NULL);
		WaitPort(&proc->pr_MsgPort);
		wbmsg = (struct WBStartup *)GetMsg(&proc->pr_MsgPort);
	}

	__WBenchMsg = wbmsg; // this variable is used in __ctor_arg_init()
	__exit_blocked = FALSE; // this will push exit() and similar functions to longjmp to target set by setjmp()

	// We open the required libraries
	// The utility library is used for the call of the CallHookPkt function
	IntuitionBase = (struct IntuitionBase *)OpenLibrary((CONST_STRPTR)"intuition.library",0L);
	DOSBase = (struct DosLibrary *)OpenLibrary((CONST_STRPTR)"dos.library",0L);
	UtilityBase = (struct UtilityBase *)OpenLibrary((CONST_STRPTR)"utility.library",0L);
	GfxBase = (struct GfxBase *)OpenLibrary("graphics.library",0);
	CxBase = OpenLibrary("commodities.library",0);
	IconBase = OpenLibrary("icon.library",0);

	if(IntuitionBase && DOSBase && UtilityBase && GfxBase && CxBase && IconBase)
	{
		__UtilityBase = UtilityBase; // we need to add this because of macro DECLARE_UTILITYBASE()

		if(setjmp(__exit_jmp_buf) != 0)
			goto out; // target for exit

		fcntCallCtor(); // call the constructors

		//Printf("Testing clib2...\n");
		// To test clib, you will have to put -lc before -lunix in the static_libs variable of the Makefile
		//Printf("clib functions %s\n",(long unsigned int)((testClibFunctions()==0)?"OK":"FAIL"));
		//Printf("math functions %s\n",(long unsigned int)((testMathFunctions()==0)?"OK":"FAIL"));
		// To test unixlib, you will have to put -lunix before -lc in the static_libs variable of the Makefile
		//Printf("unixlib functions %s\n",(long unsigned int)((testUnixlibFunctions()==0)?"OK":"FAIL"));
		//Printf("netlib functions %s\n",(long unsigned int)((testNetlibFunctions()==0)?"OK":"FAIL"));
		//Printf("debuglib functions %s\n",(long unsigned int)((testDebuglibFunctions()==0)?"OK":"FAIL"));
		//Printf("Testing amigalib...\n");
		//Printf("Fast floating point functions %s\n",(long unsigned int)((testFFP()==0)?"OK":"FAIL"));
		//Printf("Exec support functions %s\n",(long unsigned int)((testExecSupportFunctions()==0)?"OK":"FAIL"));
		//Printf("Random functions %s\n",(long unsigned int)((testRandomFunctions()==0)?"OK":"FAIL"));
		//Printf("Graphic support functions %s\n",(long unsigned int)((testGraphicSupportFunctions()==0)?"OK":"FAIL"));
		//Printf("Timer support functions %s\n",(long unsigned int)((testTimerSupportFunctions()==0)?"OK":"FAIL"));
		//Printf("Commodities support functions %s\n",(long unsigned int)((testCommoditiesFunctions()==0)?"OK":"FAIL"));
		//Printf("Arexx support functions %s\n",(long unsigned int)((testArexxFunctions()==0)?"OK":"FAIL"));
		//Printf("ACrypt function %s\n",(long unsigned int)((testACryptFunction()==0)?"OK":"FAIL"));
		//Printf("Hook and boopsi support functions %s\n",(long unsigned int)((testHookBoopsiFunctions()==0)?"OK":"FAIL"));
		//Printf("Multiplication and division functions %s\n",(long unsigned int)((testMulDivFunctions()==0)?"OK":"FAIL"));

		out:
		fcntCallDtor(); // call the destructors
	}

	// Before leaving, we close all libraries
	cleanUP();

	if(wbmsg != NULL)
	{
		// If needed, we reply to the workbench message
		Forbid();
		ReplyMsg((struct Message *)wbmsg);
	}

	return(0);
}

__attribute__((used)) __attribute__((section(".text.unlikely"))) void _start(int argc, char **argv)
{
	main(argc,argv);
}