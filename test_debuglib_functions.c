// Checks the debuglib support functions
#include <stdio.h>
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <devices/serial.h>
#include <proto/dos.h>
#include <dos/dostags.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <debug_headers.h>

#define BUFFER_SIZE 256
#define OPTION_TRACE_DEBUGLIB 0
#define ASM __asm

static WORD errCnt;
static char buffer1[BUFFER_SIZE];

// To test KDoFmt
__attribute__((noinline)) void put_char_routine(char c,APTR output)
{
	register volatile char _d0 ASM("d0") = c;
	register volatile APTR _a3 ASM("a3") = output;

	__asm volatile (
		"move.b  %%d0,(%%a3)+\n"
	: // do and a3 are not declared here, to allow their modification
	: 
	: "cc", "memory");
}

// To test KPutFmt
__attribute__((noinline)) void KPutFmt_test(const char *format, ...)
{
	va_list arg;

	va_start(arg,format);
	kvprintf(format,arg);
	va_end(arg);
}

// To test kvprintf
__attribute__((noinline)) void kvprintf_test(const char *format, ...)
{
	va_list arg;

	va_start(arg,format);
	kvprintf(format,arg);
	va_end(arg);
}

// To test KVPrintF
__attribute__((noinline)) void KVPrintF_test(const char *format, ...)
{
	va_list arg;

	va_start(arg,format);
	KVPrintF(format,arg);
	va_end(arg);
}

WORD testDebuglibFunctions(void)
{
	errCnt = 0;

	// Test cmpstrexec
	if(cmpstrexec("alph","alpha") > 0) errCnt++;
	if(cmpstrexec("alpha","alph") < 0) errCnt++;
	if(cmpstrexec("alpha","alpha") != 0) errCnt++;

	// Test KCmpStr
	if(KCmpStr("alph","alpha") > 0) errCnt++;
	if(KCmpStr("alpha","alph") < 0) errCnt++;
	if(KCmpStr("alpha","alpha") != 0) errCnt++;

	// Test KDoFmt
	char *strTable[] = {"Hello","world"};
	KDoFmt("%s %s!",(APTR)strTable,(APTR)put_char_routine,(APTR)buffer1);
	if(strcmp(buffer1,"Hello world!")) errCnt++;

	// Test kputc, kputch, KPutCh, kputchar, KPutChar, KPutS, kputs, kputstr, KPutStr, kprintf, KPrintF, KPutFmt, kvprintf, KVPrintF
	// To test these functions, please follow the below procedure :
	// a. The first time, once the program asks you to press Ctrl-C, do so
	// b. Open UAE configuration window by pressing F12 and go to IO ports
	// c. Under Serial Port, choose TCP://0.0.0.0:1234, select Direct and press OK
	// d. Open a Telnet session on port 1234 by typing "telnet localhost 1234". For Windows, if needed, activate Telnet following the instructions on page https://www.makeuseof.com/enable-telnet-windows/.
	// For MacOS, you can install it through Homebrew (see https://osxdaily.com/2018/07/18/get-telnet-macos/). For Linux, it should natively come with your distribution
	// e. Go back to UAE and in the shell type a.exe to restart the program: you should see all chars and messages in the Telnet window. You will then be asked to type some chars and numbers in the Telnet window
	// f. At the end, close the UAE window
	// Note: after the test, UAE may signal a (false) read error on the Workench disk. This is due to the fact that we modified its configuration while running
	kputc('a');
	kputch('b');
	KPutCh('c');
	kputchar('d');
	KPutChar('e');
	KPutS("alpha");
	kputs("beta");
	kputstr("gamma");
	KPutStr("delta");
	kprintf("%s %s","1234","5678");
	KPrintF("%s %s","789A","BCDE");
	KPutFmt_test("%s %s","FGHI","JKLM");
	kvprintf_test("%s %s","NOPQ","RSTU");
	KVPrintF_test("%s %s","VWXY","Z0123");

	// Test kgetc, kgetch, kgetch, KGetChar, KMayGetCh, KMayGetChar, kgetnum, KGetNum
	// To test these functions, please follow the above procedure and follow the instrutions on the screen
	LONG cc;
	printf("Please be prepared to type some chars and numbers in the Telnet window or press Ctrl-C now (see details in the comments of the code)\n");
	Delay(500);
	printf("Please type a char in the telnet window\n");
	printf("You typed %c\n",(char)kgetc());
	printf("Please type a char\n");
	printf("You typed %c\n",(char)kgetch());
	printf("Please type a char\n");
	printf("You typed %c\n",(char)kgetch());
	printf("Please type a char\n");
	printf("You typed %c\n",(char)KGetChar());
	printf("Please type a char\n");
	do {cc = KMayGetCh();} while(cc == 0xFF || cc < 0); // looks like some versions of KMayGetChar only return -1 as a char
	printf("You typed %c\n",cc);
	printf("Please type a char\n");
	do {cc = KMayGetChar();} while(cc == 0xFF || cc < 0); // looks like some versions of KMayGetChar only return -1 as a char
	printf("You typed %c\n",cc);
	printf("Please type a number followed by Enter\n");
	printf("You typed %d\n",(char)kgetnum());
	printf("Please type a number followed by Enter\n");
	printf("You typed %d\n",(char)KGetNum());

	return(errCnt);
}