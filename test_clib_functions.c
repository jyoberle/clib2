// Checks the clib support functions
#include <stdarg.h>
#include <stdio.h>
#include <stdio_headers.h>
#include <exec/types.h>
#include <proto/exec.h>
#include <exec/tasks.h>
#include <proto/dos.h>
#include <devices/input.h>
#include <clib/alib_protos.h>
#include <locale_headers.h>
#include <stdlib_constructor.h>
#include <stdlib_headers.h>
#include <dirent.h>
#include <macros.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ftw.h>
#include <inttypes.h>
#include <sys/mount.h>
#include <setjmp.h>
#include <sys/uio.h>
#include <ulimit.h>
#include <errno.h>
#include <sys/utsname.h>
#include <libgen.h>

#define RAM_PATH "RAM:"
#define RAM_DISK "Ram Disk:"
#define RAM_TEST_FILE "RAM:test_file"
#define RAM_DISK_TEST_FILE "Ram Disk:test_file"
#define RAM_TEST_HARD_LINK "RAM:test_file_hard_link"
#define NEW_RAM_TEST_FILE "RAM:new_test_file"
#define RAM_TEMP_FILE "RAM:temp_file_XXXXXX"
#define RAM_TEMP_FILE_BIS "RAM:temp_file_bis_XXXXXX"
#define RAM_TEST_DIR "RAM:test_dir"
#define RAM_TEMP_DIR "RAM:temp_dir_XXXXXX"
#define DH0_TEST_FILE "DH0:test_file"
#define DH0_TEST_SOFT_LINK "DH0:test_file_soft_link"
#define TEST_STRING_1 "In the beginning "
#define TEST_STRING_2 "God created the heaven and the Earth."
#define TEST_STRING_1_2 "In the beginning God created the heaven and the Earth."
#define BUFFER_SIZE 256
#define FTW_NFTW_DEPTH 4
#define DOS_TYPE "DOS"
#define ERROR_STR_TEST "Test of error string"
#define PIPE_CMD "C:echo "
#define PIPE_TEST_STR "pipe test"
#define LOVE_STRING "JOB loves Amiga"
#define AMIGA_STRING "Amiga"
#define NBR1 12
#define NBR2 24
#define SETENV_VAR_NAME "AMIGA"
#define SETENV_VAR_VALUE "for ever"
#define UNKNOWN_ERROR "Unknown error"
#define UID 4
#define GID 8
#define TRUNCATE_LENGTH 3
#define LOCKF_LEN 4
#define OPTION_TRACE_CLIB 0

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

extern LONG __divsi3(LONG dividend,LONG divisor);
extern LONG __divsi4(LONG dividend,LONG divisor);
extern LONG __modsi3(LONG dividend,LONG divisor);
extern LONG __mulsi3(LONG Arg1,LONG Arg2);
extern ULONG __udivsi3(ULONG dividend,ULONG divisor);
extern ULONG __udivsi4(ULONG dividend,ULONG divisor);
extern ULONG __umodsi3(ULONG dividend,ULONG divisor);

static char buffer1[BUFFER_SIZE],buffer2[BUFFER_SIZE],buffer3[BUFFER_SIZE],buffer4[BUFFER_SIZE],test_strings[BUFFER_SIZE],file_buffer[BUFSIZ],file_buffer2[BUFSIZ];
static BOOL SIGINT_caught;
static FILE *file_ptr;
static WORD long_jmp_var;
static jmp_buf env;
static WORD errCnt;

// ftw callback function
static int ftw_callback(const char *name,const struct stat *stat,int flags)
{
#if OPTION_TRACE_CLIB
	Printf("ftw: name = %s, st_size = %lu, flags = %lu\n",name,stat->st_size,flags);
#endif
	return(0);
}

// nftw callback function
static int nftw_callback(const char *name,const struct stat *stat,int flags,struct FTW *ftw)
{
#if OPTION_TRACE_CLIB
	Printf("nftw: name = %s, st_size = %lu, flags = %lu, base = %u, level = %u\n",name,stat->st_size,flags,ftw->base,ftw->level);
#endif
	return(0);
}

// Start wait command as a CLI process
static LONG fcntStartWait(void)
{
	struct TagItem stags[4];

	stags[0].ti_Tag = SYS_Input;
	stags[0].ti_Data = (ULONG)Open("NIL:",MODE_READWRITE);
	stags[1].ti_Tag = SYS_Output;
	stags[1].ti_Data = (ULONG)Open("NIL:",MODE_READWRITE);
	stags[2].ti_Tag = SYS_Asynch;
	stags[2].ti_Data = (ULONG)TRUE;
	stags[3].ti_Tag = TAG_END;
	SystemTagList("C:wait 10",stags);
	Delay(50); // wait for command to start

	return(MaxCli() - 1); // CLI number
}

// Signal handler
static void catch_int(int sig_num)
{
	if(sig_num == SIGINT)
		SIGINT_caught = TRUE;
}

// Process used to lock (and unlock) a file
static VOID fcntProcLockFile(void)
{
	flockfile(file_ptr);
	Delay(500); // delay before releasing lock
	funlockfile(file_ptr);
}

// To test vasprintf
__attribute__((noinline)) int vasprintf_test(char **ret, const char *format, ...)
{
	int result = EOF;
	va_list arg;

	va_start(arg,format);
	result = vasprintf(ret,format,arg);
	va_end(arg);

	return(result);
}

// To test vfprintf
__attribute__((noinline)) int vfprintf_test(FILE *file_ptr,const char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vfprintf(file_ptr,format,arg);
	va_end(arg);

	return(result);
}

// To test vfscanf
__attribute__((noinline)) int vfscanf_test(FILE *file_ptr,const char *format, ...)
{
	int result = EOF;
	va_list arg;

	va_start(arg,format);
	result = vfscanf(file_ptr,format,arg);
	va_end(arg);

	return(result);
}

// To test vprintf
__attribute__((noinline)) int vprintf_test(const char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vprintf(format,arg);
	va_end(arg);

	return(result);
}

// To test vsnprintf
__attribute__((noinline)) int vsnprintf_test(char *s,size_t n,char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vsnprintf(s,n,format,arg);
	va_end(arg);

	return(result);
}

// To test vsprintf
__attribute__((noinline)) int vsprintf_test(char *s,char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vsprintf(s,format,arg);
	va_end(arg);

	return(result);
}

// To test vsscanf
__attribute__((noinline)) int vsscanf_test(char *s,char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vsscanf(s,format,arg);
	va_end(arg);

	return(result);
}

// To test vscanf
__attribute__((noinline)) int vscanf_test(char *format, ...)
{
	int result = EOF;
	va_list arg;
	
	va_start(arg,format);
	result = vscanf(format,arg);
	va_end(arg);

	return(result);
}

// Signal handler for SIGABRT
static void catch_SIGABRT(int sig_num)
{
	if(sig_num == SIGABRT)
		Printf("catch_SIGABRT called\n");
}

// Function called at exit
static void exit_function(void)
{
	Printf("Exit function called\n");
}

// To test bsearch
#define ARRAY_SIZE(arr)  (sizeof((arr)) / sizeof((arr)[0]))
#define THIRD_MONTH "mar"

struct mi {
	int nr;
	const char *name;
};

static struct mi  months[] = {
	{ 1, "jan" }, { 2, "feb" }, { 3, "mar" }, { 4, "apr" },
	{ 5, "may" }, { 6, "jun" }, { 7, "jul" }, { 8, "aug" },
	{ 9, "sep" }, {10, "oct" }, {11, "nov" }, {12, "dec" }
};

static int compmi(const void *m1, const void *m2)
{
	const struct mi *mi1 = m1;
	const struct mi *mi2 = m2;

	return strcmp(mi1->name, mi2->name);
}

// To test longjmp
static void fcnt_long_jmp1(void)
{
	long_jmp_var = 1;
	longjmp(env,10);
	long_jmp_var = 2;
}

WORD testClibFunctions(void)
{
	errCnt = 0;

	// Test stdio functions requiring an input from the user
	// Test getchar, putchar, getchar_unlocked, putchar_unlocked, gets, puts
	int c_int;
	char *str_test;

	printf("Please type a letter: \n");

	if((c_int = getchar()) == EOF)
	{
		errCnt++;
	}
	else
	{
		printf("You typed ");

		if(putchar((char)c_int) == EOF)
			errCnt++;

		printf("\n");
	}

	getchar(); // for \n
	printf("Please type another letter: \n");

	if((c_int = getchar_unlocked()) == EOF)
	{
		errCnt++;
	}
	else
	{
		printf("You typed ");

		if(putchar_unlocked((char)c_int) == EOF)
			errCnt++;

		printf("\n");
	}

	getchar(); // for \n
	memset(buffer1,0,BUFFER_SIZE);
	printf("Please type a string: \n");

	if((str_test = gets(buffer1)) == NULL)
	{
		errCnt++;
	}
	else
	{
		printf("You typed ");

		if(puts(str_test) == EOF)
			errCnt++;
	}

	// Test scanf, vscanf
	int nb1,nb2;

	printf("Please type two numbers separated by a space:\n");

	if(scanf("%d %d",&nb1,&nb2) != 2)
	{
		errCnt++;
		printf("scanf error\n");
	}
	else
	{
		printf("%d + %d = %d\n",nb1,nb2,nb1 + nb2);
	}

	printf("Please type two other numbers separated by a space:\n");

	if(vscanf_test("%d %d",&nb1,&nb2) != 2)
	{
		errCnt++;
		printf("vscanf error\n");
	}
	else
	{
		printf("%d + %d = %d\n",nb1,nb2,nb1 + nb2);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE1 errCnt = %u\n",errCnt);
#endif

	// Test isalnum
	if(!isalnum('J')) errCnt++;
	if(!isalnum('5')) errCnt++;
	if(isalnum(0xFF)) errCnt++;

	// Test isalpha
	if(!isalpha('O')) errCnt++;
	if(!isalpha('o')) errCnt++;
	if(isalpha('5')) errCnt++;

	// Test isascii
	if(!isascii('B')) errCnt++;
	if(!isascii('0')) errCnt++;
	if(isascii(128)) errCnt++;

	// Test isblank
	if(!isblank('\t')) errCnt++;
	if(!isblank(' ')) errCnt++;
	if(isblank('A')) errCnt++;

	// Test iscntrl
	if(!iscntrl('\0')) errCnt++;
	if(!iscntrl(127)) errCnt++;
	if(iscntrl('A')) errCnt++;

	// Test isdigit
	if(!isdigit('4')) errCnt++;
	if(!isdigit('2')) errCnt++;
	if(isdigit('A')) errCnt++;

	// Test isgraph
	if(!isgraph('A')) errCnt++;
	if(!isgraph(126)) errCnt++;
	if(isgraph(' ')) errCnt++;

	// Test islower
	if(!islower('j')) errCnt++;
	if(!islower('o')) errCnt++;
	if(islower('J')) errCnt++;

	// Test isprint
	if(!isprint('A')) errCnt++;
	if(!isprint('o')) errCnt++;
	if(isprint(0xFF)) errCnt++;

	// Test ispunct
	if(!ispunct('!')) errCnt++;
	if(!ispunct('?')) errCnt++;
	if(ispunct('B')) errCnt++;

	// Test isspace
	if(!isspace(' ')) errCnt++;
	if(!isspace('\r')) errCnt++;
	if(isspace('B')) errCnt++;

	// Test isupper
	if(!isupper('A')) errCnt++;
	if(!isupper('B')) errCnt++;
	if(isupper('c')) errCnt++;

	// Test isxdigit
	if(!isxdigit('C')) errCnt++;
	if(!isxdigit('d')) errCnt++;
	if(isxdigit('G')) errCnt++;

	// Test tolower
	if(tolower('C') != 'c') errCnt++;
	if(tolower('B') != 'b') errCnt++;
	if(tolower('a') != 'a') errCnt++;

	// Test toupper
	if(toupper('c') != 'C') errCnt++;
	if(toupper('b') != 'B') errCnt++;
	if(toupper('A') != 'A') errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE2 errCnt = %u\n",errCnt);
#endif

	// Test dir functions (opendir, readdir, rewinddir, closedir)
	DIR *dir;
	struct dirent *dir_entry;
	int file_desc;

	Printf("Listing twice RAM: content...\n");

	if((dir = opendir(RAM_PATH)) == NULL)
	{
		errCnt++;
	}
	else
	{
		while((dir_entry = readdir(dir)) != NULL)
		{
			Printf("%s\n",dir_entry->d_name);
		}

		rewinddir(dir);

		while((dir_entry = readdir(dir)) != NULL)
		{
			Printf("%s\n",dir_entry->d_name);
		}

		closedir(dir);
	}

	// Test fcntl functions (creat, write, close, open, lseek, read, fcntl)
	size_t test_str1_len = strlen(TEST_STRING_1);
	size_t test_str2_len = strlen(TEST_STRING_2);

#if OPTION_TRACE_CLIB
	printf("TRACE3 errCnt = %u\n",errCnt);
#endif

	// Write first string in file
	if((file_desc = creat(RAM_TEST_FILE,S_IRWXU)) == ERROR) // mode is not used
	{
		errCnt++;
	}
	else
	{
		test_str1_len = strlen(TEST_STRING_1);

		if((size_t)write(file_desc,TEST_STRING_1,test_str1_len) != test_str1_len)
		{
			errCnt++;
		}

		if(close(file_desc) == ERROR)
		{
			errCnt++;
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE4 errCnt = %u\n",errCnt);
#endif

	// Write second string in file
	if((file_desc = open(RAM_TEST_FILE,O_WRONLY | O_APPEND)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(lseek(file_desc,0L,SEEK_END) == SEEK_ERROR)
		{
			errCnt++;
		}
		else
		{
			if((size_t)write(file_desc,TEST_STRING_2,test_str2_len) != test_str2_len)
			{
				errCnt++;
			}

			if(close(file_desc) == ERROR)
			{
				errCnt++;
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE5 errCnt = %u\n",errCnt);
#endif

	// Read strings back (and check fcntl)
	if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		memset(buffer1,0,BUFFER_SIZE);

		if(read(file_desc,buffer1,test_str1_len + test_str2_len) != (ssize_t)(test_str1_len + test_str2_len))
		{
			errCnt++;
		}

		if(fcntl(file_desc,F_GETFL) != 0)
		{
			errCnt++;
		}

		if(close(file_desc) == ERROR)
		{
			errCnt++;
		}
	}

	strcpy(test_strings,TEST_STRING_1);
	strcat(test_strings,TEST_STRING_2);

	if(strncmp(buffer1,test_strings,test_str1_len + test_str2_len))
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE6 errCnt = %u\n",errCnt);
#endif

	// Test ftw and nftw functions
	if(ftw(RAM_PATH,ftw_callback,FTW_NFTW_DEPTH) != 0)
		errCnt++;

	if(nftw(RAM_PATH,nftw_callback,FTW_NFTW_DEPTH,FTW_PHYS) != 0)
		errCnt++;

#if defined(USE_64_BIT_INTS) // long long is not supported
	// Test imaxdiv, imaxabs, strtoimax, strtoumax
	intmax_t a = 9223372036854775807,b = 223372036854775807,c;
	uintmax_t d = 18446744073709551615,e;
	imaxdiv_t result;

	result = imaxdiv(a,b);

	if(result.quot != (intmax_t)41 || result.rem != (intmax_t)65118525808967720)
		errCnt++;

	c = imaxabs(-a);

	if(a != c)
		errCnt++;

	c = strtoimax("9223372036854775807",NULL,10);

	if(a != c)
		errCnt++;

	e = strtoumax("18446744073709551615",NULL,10);

	if(d != e)
		errCnt++;
#endif

#if OPTION_TRACE_CLIB
	printf("TRACE7 errCnt = %u\n",errCnt);
#endif

	// Test basename and dirname
	if(strcmp(basename("RAM:a/b/c/d/e.txt"),"e.txt"))
		errCnt++;

	if(strcmp(dirname("RAM:a/b/c/d/e.txt"),"RAM:a/b/c/d"))
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE8 errCnt = %u\n",errCnt);
#endif

	// Test locale functions (setlocale, localeconv)
	struct lconv *lconv;

	if(setlocale(LC_ALL,NULL) == NULL) // default locale
	{
		errCnt++;
	}
	else
	{
		if(strcmp(localeconv()->decimal_point,"."))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE9 errCnt = %u\n",errCnt);
#endif

	// Test fstatfs and statfs
	struct statfs stat_fs;

	if(statfs(RAM_TEST_FILE,&stat_fs) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(strncmp(stat_fs.f_fstypename,DOS_TYPE,strlen(DOS_TYPE)))
			errCnt++;
	}

	if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(fstatfs(file_desc,&stat_fs) == ERROR)
		{
			errCnt++;
		}
		else
		{
			if(strncmp(stat_fs.f_fstypename,DOS_TYPE,strlen(DOS_TYPE)))
				errCnt++;

			close(file_desc);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE10 errCnt = %u\n",errCnt);
#endif

	// Test signals (kill, raise, sigaddset, sigblock, sigemptyset, sigmask, signal, sigprocmask, sigsetmask)
	ULONG maxCLI;
	sigset_t sigset;

	maxCLI = fcntStartWait(); // start a CLI wait process
	kill((pid_t)FindCliProc(maxCLI),SIGTERM);
	Delay(250); // wait for command to end

	if(MaxCli() != maxCLI) // the CLI should have ended
		errCnt++;

	SIGINT_caught = FALSE;
	signal(SIGINT,catch_int);
	raise(SIGINT);
	Delay(250); // wait for signal to be transmitted

	if(SIGINT_caught == FALSE)
		errCnt++;

	sigset = 1 << SIGFPE;
	sigemptyset(&sigset);

	if(sigset != 0L)
		errCnt++;

	sigset = sigmask(SIGILL);

	if(sigset != 1 << SIGILL)
		errCnt++;

	sigaddset(&sigset,SIGSEGV);

	if(sigset != ((1 << SIGILL) | (1 << SIGSEGV)))
		errCnt++;

	sigsetmask(sigmask(SIGINT));
	SIGINT_caught = FALSE;
	signal(SIGINT,catch_int);
	raise(SIGINT);
	Delay(250); // wait for signal to be transmitted

	if(SIGINT_caught != FALSE) // signal must not be transmitted as it is blocked
		errCnt++;

	sigset = sigmask(SIGINT);
	sigprocmask(SIG_UNBLOCK,&sigset,NULL); // unblock SIGINT

	SIGINT_caught = FALSE;
	signal(SIGINT,catch_int);
	raise(SIGINT);
	Delay(250); // wait for signal to be transmitted

	if(SIGINT_caught == FALSE) // signal should be transmitted
		errCnt++;

	sigblock(sigmask(SIGINT));
	SIGINT_caught = FALSE;
	signal(SIGINT,catch_int);
	raise(SIGINT);
	Delay(250); // wait for signal to be transmitted

	if(SIGINT_caught != FALSE) // signal must not be transmitted as it is blocked
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE11 errCnt = %u\n",errCnt);
#endif

	// Test stat functions (chmod, stat, lstat, fchmod, fstat, umask, mkdir, rmdir)
	struct stat file_stat;

	if(chmod(RAM_TEST_FILE,S_IWUSR | S_IRUSR | S_IXUSR) == ERROR) // set read, write and execute for user
		errCnt++;

	if(stat(RAM_TEST_FILE,&file_stat) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(file_stat.st_mode & S_IXUSR == 0L)
			errCnt++;
	}

	if(lstat(RAM_TEST_FILE,&file_stat) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(file_stat.st_mode & S_IXUSR == 0L)
			errCnt++;
	}

	if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(fchmod(file_desc,S_IWUSR | S_IRUSR) == ERROR) // do not set execute
			errCnt++;

		if(fstat(file_desc,&file_stat) == ERROR)
		{
			errCnt++;
		}
		else
		{
			if(file_stat.st_mode & S_IXUSR != 0L)
				errCnt++;
		}

		close(file_desc);
	}

	umask(S_IRWXU);

	if(umask(S_IRWXG) != S_IRWXU)
		errCnt++;

	if(mkdir(RAM_TEST_DIR,S_IRWXU) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((dir = opendir(RAM_TEST_DIR)) == NULL)
		{
			errCnt++;
		}
		else
		{
			closedir(dir);

			if(rmdir(RAM_TEST_DIR) == ERROR)
			{
				errCnt++;
			}
			else
			{
				if((dir = opendir(RAM_TEST_DIR)) != NULL)
					errCnt++;
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE12 errCnt = %u\n",errCnt);
#endif

	// Test stdio functions
	char *ret;

	// Test asprintf
	if(asprintf(&ret,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(test_str1_len + test_str2_len))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(ret,test_strings,test_str1_len + test_str2_len))
			errCnt++;

		free(ret);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE13 errCnt = %u\n",errCnt);
#endif

	// Test fopen, fprintf, fflush, fclose, fread
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(fprintf(file_ptr,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(test_str1_len + test_str2_len))
		{
			errCnt++;
		}
		else
		{
			if(fflush(file_ptr) != 0)
				errCnt++;

			fclose(file_ptr);
			file_ptr = NULL;

			if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(fread(buffer1,test_str1_len + test_str2_len,1,file_ptr) != 1)
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,test_strings,test_str1_len + test_str2_len))
					{
						errCnt++;
					}
				}

				fclose(file_ptr);
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE14 errCnt = %u\n",errCnt);
#endif

	// Test snprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(snprintf(buffer1,test_str1_len + 1,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(test_str1_len + test_str2_len)) // + 1 to include the ending '\0'
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,test_strings,strlen(test_str1_len)))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE15 errCnt = %u\n",errCnt);
#endif

	// Test sprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(sprintf(buffer1,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(test_str1_len + test_str2_len))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,test_strings,test_str1_len + test_str2_len))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE16 errCnt = %u\n",errCnt);
#endif

	// Test vprintf
	printf("Testing vprintf:");
	if(vprintf_test(buffer1,BUFFER_SIZE,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)strlen(TEST_STRING_1_2))
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE17 errCnt = %u\n",errCnt);
#endif

	// Test fgetc, fgetpos, fsetpos, fgets, fread, feof, perror
	if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
	{
		errCnt++;
	}
	else
	{
		fpos_t pos;

		if(fgetc(file_ptr) != (int)test_strings[0])
			errCnt++;

		if(fgetpos(file_ptr,&pos) != OK)
		{
			errCnt++;
		}
		else
		{
			if(pos != (fpos_t)1)
			{
				errCnt++;
			}
		}

		pos = (fpos_t)0;

		if(fsetpos(file_ptr,&pos)!= OK)
		{
			errCnt++;
		}
		else
		{
			memset(buffer1,0,BUFFER_SIZE);

			if(fgets(buffer1,test_str1_len + 1,file_ptr) == NULL)
			{
				errCnt++;
			}
			else
			{
				if(strncmp(buffer1,test_strings,test_str1_len))
					errCnt++;
			}
		}

		memset(buffer1,0,BUFFER_SIZE);

		if(fread(buffer1,BUFFER_SIZE,1,file_ptr) != 0) // read beyond the file
		{
			errCnt++;
		}
		else
		{
			if(feof(file_ptr) == 0)
			{
				errCnt++;
			}
			else
			{
				clearerr(file_ptr); // clear EOF

				if(feof(file_ptr) != 0)
					errCnt++;
			}
		}

		fclose(file_ptr);

		fread(buffer1,BUFFER_SIZE,1,file_ptr); // try to read a closed file

		if(ferror(file_ptr) == 0)
			errCnt++;

		perror("Error for trying to read a closed file");
	}

#if OPTION_TRACE_CLIB
	printf("TRACE18 errCnt = %u\n",errCnt);
#endif

	// Test ftrylockfile
	if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
	{
		errCnt++;
	}
	else
	{
		// Create a new process to lock/unlock the file
		struct TagItem stags[2];

		stags[0].ti_Tag = NP_Entry;
		stags[0].ti_Data = (ULONG)fcntProcLockFile;
		stags[1].ti_Tag = TAG_END;

		if(CreateNewProc(stags) == NULL)
		{
			errCnt++;
		}
		else
		{
			Delay(250); // wait for process to start

			if(ftrylockfile(file_ptr) == OK)
				errCnt++;

			Delay(1000); // wait for process to end by itself
		}

		fclose(file_ptr);
		file_ptr = NULL;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE19 errCnt = %u\n",errCnt);
#endif

	// Test fputc, putc, putc_unlocked, fputs, fputc, fscanf, fseek, ftell, fscanf, fseeko, ftello, rewind
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		strcpy(buffer2,LOVE_STRING);

		if(fputc(buffer2[0],file_ptr) != buffer2[0])
			errCnt++;

		if(putc(buffer2[1],file_ptr) != buffer2[1])
			errCnt++;

		if(putc_unlocked(buffer2[2],file_ptr) != buffer2[2])
			errCnt++;

		if(fputs(&buffer2[3],file_ptr) < 0)
			errCnt++;

		if(fputc('\0',file_ptr) != '\0')
			errCnt++;		

		fclose(file_ptr);

		if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
		{
			errCnt++;
		}
		else
		{
			memset(buffer1,0,BUFFER_SIZE);

			if(fscanf(file_ptr,"%s",&buffer1) != 1)
				errCnt++;

			if(strncmp(buffer1,LOVE_STRING,3)) // first word
				errCnt++;

			if(fseek(file_ptr,-2L,SEEK_CUR) != 0)
				errCnt++;

			if(ftell(file_ptr) != 1L)
				errCnt++;

			memset(buffer1,0,BUFFER_SIZE);

			if(fscanf(file_ptr,"%s",&buffer1) != 1)
				errCnt++;

			if(strncmp(buffer1,&buffer2[1],2))
				errCnt++;

			if(fseeko(file_ptr,0L,SEEK_SET) != 0)
				errCnt++;

			if(ftello(file_ptr) != 0L)
				errCnt++;

			memset(buffer1,0,BUFFER_SIZE);

			if(fscanf(file_ptr,"%s",&buffer1) != 1)
				errCnt++;

			if(strncmp(buffer1,buffer2,3))
				errCnt++;

			if(fseek(file_ptr,0L,SEEK_END) != 0)
				errCnt++;

			rewind(file_ptr);

			memset(buffer1,0,BUFFER_SIZE);

			if(fscanf(file_ptr,"%s",&buffer1) != 1)
				errCnt++;

			if(strncmp(buffer1,buffer2,3))
				errCnt++;

			fclose(file_ptr);
			file_ptr = NULL;
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE20 errCnt = %u\n",errCnt);
#endif

	// Test fwrite, getc, getc_unlocked, ungetc, fread
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(fwrite(TEST_STRING_1,test_str1_len,1,file_ptr) != 1)
			errCnt++;

		if(fwrite(TEST_STRING_2,test_str2_len,1,file_ptr) != 1)
			errCnt++;

		fclose(file_ptr);

		if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
		{
			errCnt++;
		}
		else
		{
			if(getc(file_ptr) != test_strings[0])
				errCnt++;

			if(getc_unlocked(file_ptr) != test_strings[1])
				errCnt++;

			if(ungetc(test_strings[1],file_ptr) != test_strings[1])
				errCnt++;

			memset(buffer1,0,BUFFER_SIZE);

			if(fread(buffer1,test_str1_len + test_str2_len - 1,1,file_ptr) != 1)
				errCnt++;

			if(strncmp(buffer1,&test_strings[1],test_str1_len + test_str2_len - 1))
				errCnt++;

			fclose(file_ptr);
			file_ptr = NULL;
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE21 errCnt = %u\n",errCnt);
#endif

	// Test popen, pclose (needs "cmdList":"C:Mount PIPE:" in launch.json)
	FILE *pipe;
	strcpy(buffer1,PIPE_CMD);
	strcat(buffer1,PIPE_TEST_STR);

	if((pipe = popen(buffer1,"r")) == NULL)
	{
		errCnt++;
	}
	else
	{
		memset(buffer1,0,BUFFER_SIZE);
		fread(buffer1,BUFFER_SIZE,1,pipe);

		if(strncmp(buffer1,PIPE_TEST_STR,strlen(PIPE_TEST_STR)))
			errCnt++;

		pclose(pipe);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE22 errCnt = %u\n",errCnt);
#endif

	// Test mkdir, remove, opendir, closedir
	if(mkdir(RAM_TEST_DIR,S_IRWXU) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(remove(RAM_TEST_DIR) != 0)
		{
			errCnt++;
		}
		else
		{
			if((dir = opendir(RAM_TEST_DIR)) != NULL)
			{
				errCnt++;
				closedir(dir);
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE24 errCnt = %u\n",errCnt);
#endif

	// Test rename
	if(rename(RAM_TEST_FILE,NEW_RAM_TEST_FILE) != 0)
	{
		errCnt++;
	}
	else
	{
		if((file_ptr = fopen(NEW_RAM_TEST_FILE,"r")) == NULL)
		{
			errCnt++;
		}
		else
		{		
			fclose(file_ptr);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE25 errCnt = %u\n",errCnt);
#endif

	// Test setbuf
	if((file_ptr = fopen(NEW_RAM_TEST_FILE,"r")) == NULL)
	{
		errCnt++;
	}
	else
	{
		setbuf(file_ptr,file_buffer);

		if(getc(file_ptr) != test_strings[0])
			errCnt++;

		if(setvbuf(file_ptr,file_buffer2,IOBF_BUFFER_MODE_FULL,BUFSIZ) != OK)
			errCnt++;

		if(getc(file_ptr) != test_strings[1])
			errCnt++;

		fclose(file_ptr);
		file_ptr = NULL;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE26 errCnt = %u\n",errCnt);
#endif

	// Test chdir, tmpfile
	FILE *tmp_file;

	if(mkdir(RAM_TEST_DIR,S_IRWXU) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(chdir(RAM_TEST_DIR) != 0)
		{
			errCnt++;
		}
		else
		{
			if((tmp_file = tmpfile()) == NULL)
			{
				errCnt++;
			}
			else
			{
				fclose(tmp_file);
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE27 errCnt = %u\n",errCnt);
#endif

	// Test tmpnam
	char *tmp_name;

	memset(buffer1,0,BUFFER_SIZE);

	if((tmp_name = tmpnam(buffer1)) == NULL)
	{
		errCnt++;
	}
	else
	{
		printf("Temporary file name: %s\n",tmp_name);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE28 errCnt = %u\n",errCnt);
#endif

	// Test freopen
	FILE *new_file_ptr;

	if((file_ptr = fopen(NEW_RAM_TEST_FILE,"r")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if((new_file_ptr = freopen(RAM_TEST_FILE,"w",file_ptr)) == NULL)
		{
			errCnt++;
		}
		else
		{
			if(fwrite(TEST_STRING_1,test_str1_len,1,new_file_ptr) != 1)
				errCnt++;

			if(fwrite(TEST_STRING_2,test_str2_len,1,new_file_ptr) != 1)
				errCnt++;

			fclose(new_file_ptr);

			if((new_file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(fread(buffer1,test_str1_len + test_str2_len,1,new_file_ptr) != 1)
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,test_strings,test_str1_len + test_str2_len))
					{
						errCnt++;
					}
				}

				fclose(new_file_ptr);
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE29 errCnt = %u\n",errCnt);
#endif

	// Test sprintf, sscanf
	static int nbr1,nbr2;

	memset(buffer1,0,BUFFER_SIZE);

	if(sprintf(buffer1,"%d %d",NBR1,NBR2) < 0)
	{
		errCnt++;
	}
	else
	{
		if(sscanf(buffer1,"%d %d",&nbr1,&nbr2) != 2)
		{
			errCnt++;
		}
		else
		{
			if(nbr1 != NBR1 || nbr2 != NBR2)
			{
				errCnt++;
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE30 errCnt = %u\n",errCnt);
#endif

	// Test vasprintf
	char *vret;
	int len;

	if((len = vasprintf_test(&vret,"%s%s",TEST_STRING_1,TEST_STRING_2)) != (int)strlen(TEST_STRING_1_2))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(vret,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;

		free(vret);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE31 errCnt = %u\n",errCnt);
#endif

	// Test vfprintf
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(vfprintf_test(file_ptr,"%s%s",TEST_STRING_1,TEST_STRING_2) < 0)
		{
			errCnt++;
		}
		else
		{
			fclose(file_ptr);
			file_ptr = NULL;

			if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(fread(buffer1,strlen(TEST_STRING_1_2),1,file_ptr) != 1)
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
					{
						errCnt++;
					}
				}

				fclose(file_ptr);
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE32 errCnt = %u\n",errCnt);
#endif

	// Test vfscanf
	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(fwrite(LOVE_STRING,strlen(LOVE_STRING),1,file_ptr) != 1)
		{
			errCnt++;
		}
		else
		{
			fclose(file_ptr);
			file_ptr = NULL;

			if((file_ptr = fopen(RAM_TEST_FILE,"r")) == NULL)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(vfscanf_test(file_ptr,"%s",&buffer1) == EOF)
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,LOVE_STRING,3)) // first word
					{
						errCnt++;
					}
				}

				fclose(file_ptr);
				file_ptr = NULL;
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE33 errCnt = %u\n",errCnt);
#endif

	// Test vsnprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(vsnprintf_test(buffer1,BUFFER_SIZE,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)strlen(TEST_STRING_1_2))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE34 errCnt = %u\n",errCnt);
#endif

	// Test vsprintf
	memset(buffer1,0,BUFFER_SIZE);

	if(vsprintf_test(buffer1,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)strlen(TEST_STRING_1_2))
	{
		errCnt++;
	}
	else
	{
		if(strncmp(buffer1,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE35 errCnt = %u\n",errCnt);
#endif

	// Test vsscanf
	memset(buffer1,0,BUFFER_SIZE);

	if(sprintf(buffer1,"%d %d",NBR1,NBR2) < 0)
	{
		errCnt++;
	}
	else
	{
		if(vsscanf_test(buffer1,"%d %d",&nbr1,&nbr2) != 2)
		{
			errCnt++;
		}
		else
		{
			if(nbr1 != NBR1 || nbr2 != NBR2)
				errCnt++;
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE36 errCnt = %u\n",errCnt);
#endif

	// Test stdlib functions

	// Test abs
	if(abs(-25) != 25)
		errCnt++;

	// Test alloca
	if(alloca(256) == NULL)
		errCnt++;

	// Test atoi
	if(atoi("24") != 24)
		errCnt++;

	// Test atol
	if(atol("16777216") != 16777216L)
		errCnt++;

#if defined(USE_64_BIT_INTS)
	// Test atoll
	if(atoll("68719476736") != 68719476736LL)
		errCnt++;
#endif

#if OPTION_TRACE_CLIB
	printf("TRACE37 errCnt = %u\n",errCnt);
#endif

	// Test bsearch and qsort
	struct mi key;
	struct mi *res;

	qsort(months, ARRAY_SIZE(months), sizeof(months[0]), compmi);
	key.name = THIRD_MONTH;
	res = bsearch(&key, months, ARRAY_SIZE(months),sizeof(months[0]),compmi);

	if(res == NULL)
	{
		errCnt++;
	}
	else
	{
		if(res->nr != 3 || strcmp(res->name,THIRD_MONTH))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE38 errCnt = %u\n",errCnt);
#endif

	// Test calloc, free
	void *memPtr;

	if((memPtr = calloc(4,2)) == NULL)
	{
		errCnt++;
	}
	else
	{
		free(memPtr);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE39 errCnt = %u\n",errCnt);
#endif

	// Test div
	div_t resDiv = div(-64,7);

	if(resDiv.quot != -9 || resDiv.rem != -1)
		errCnt++;

	// Test divsi3
	if(__divsi3(-16777216L,439L) != -38216L)
		errCnt++;

	// Test divsi4
	if(__divsi4(-16777216L,439L) != -38216L)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE40 errCnt = %u\n",errCnt);
#endif

	// Test getenv, setenv, unsetenv
	if(setenv(SETENV_VAR_NAME,SETENV_VAR_VALUE,1) != OK) // with overwrite
	{
		errCnt++;
	}
	else
	{
		if(strcmp(getenv(SETENV_VAR_NAME),SETENV_VAR_VALUE))
			errCnt++;

		if(unsetenv(SETENV_VAR_NAME) != OK)
		{
			errCnt++;
		}
		else
		{
			if(getenv(SETENV_VAR_NAME) != NULL)
				errCnt++;
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE41 errCnt = %u\n",errCnt);
#endif

	// Test labs
	if(labs(-12346789L) != 12346789L)
		errCnt++;

	// Test llabs
	if(llabs(-68719476736LL) != 68719476736LL)
		errCnt++;

	// Test ldiv
	ldiv_t resLDiv = ldiv(-16777216L,439L);

	if(resLDiv.quot != -38216L || resLDiv.rem != -392L)
		errCnt++;

#if defined(USE_64_BIT_INTS)
	// Test lldiv
	lldiv_t resLLDiv = lldiv(68719476736LL,16777215LL);

	if(resLLDiv.quot != 4096LL || resLLDiv.rem != 4096LL)
		errCnt++;
#endif

#if OPTION_TRACE_CLIB
	printf("TRACE42 errCnt = %u\n",errCnt);
#endif

	// Test malloc, realloc
	void *memPtrRealloc;

	if((memPtr = malloc(strlen(LOVE_STRING))) == NULL)
	{
		errCnt++;
	}
	else
	{
		strcpy(memPtr,LOVE_STRING);

		if((memPtrRealloc = realloc(memPtr,strlen(LOVE_STRING))) == NULL)
		{
			errCnt++;
		}
		else
		{
			if(strcmp(memPtrRealloc,LOVE_STRING))
				errCnt++;

			free(memPtr);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE43 errCnt = %u\n",errCnt);
#endif

	// Test mkdtemp
	char *tempDir = mkdtemp(RAM_TEMP_DIR);

	if(tempDir == NULL)
	{
		errCnt++;
	}
	else
	{
		if((dir = opendir(tempDir)) == NULL)
		{
			errCnt++;
		}
		else
		{
			closedir(dir);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE44 errCnt = %u\n",errCnt);
#endif

	// Test mkstemp
	int tempFileDesc = mkstemp(RAM_TEMP_FILE);

	if(tempFileDesc == 0)
	{
		errCnt++;
	}
	else
	{
		if(close(tempFileDesc) == ERROR)
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE45 errCnt = %u\n",errCnt);
#endif

	// Test mktemp
	char tempFileName[256];
	
	strcpy(tempFileName,RAM_TEMP_FILE_BIS);

	if(strlen(mktemp(tempFileName)) == 0)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE46 errCnt = %u\n",errCnt);
#endif

	// Test modsi3
	if(__modsi3(-16777216L,439L) != -392L)
		errCnt++;

	// Test mulsi3
	if(__mulsi3(-16777216L,10L) != -167772160L)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE47 errCnt = %u\n",errCnt);
#endif

	// Test putenv
	if(putenv("AMIGA=the best") != 0)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(getenv("AMIGA"),"the best"))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE48 errCnt = %u\n",errCnt);
#endif

	// Test rand, rand_r, srand
	unsigned int seed = rand();
	int rand1,rand2;

	srand(seed);
	rand1 = rand();
	srand(seed);
	rand2 = rand();

	if(rand1 != rand2)
		errCnt++;

	if(rand() == rand())
		errCnt++;

	if(rand_r(&seed) == rand_r(&seed))
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE49 errCnt = %u\n",errCnt);
#endif

	// Test strtol
	if(strtol("-16777216",NULL,10) != -16777216L)
		errCnt++;

#if defined(USE_64_BIT_INTS)
	if(strtoll("-16777216",NULL,10) != -16777216LL)
		errCnt++;
#endif

	// Test strtoul
	if(strtoul("16777216",NULL,10) != 16777216L)
		errCnt++;

#if defined(USE_64_BIT_INTS)
	// Test strtoull
	if(strtoull("16777216",NULL,10) != 16777216LL)
		errCnt++;
#endif

#if OPTION_TRACE_CLIB
	printf("TRACE50 errCnt = %u\n",errCnt);
#endif

	// Test system
	printf("Displaying RAM: content with system...\n");

	if(system("dir RAM:") == -1)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE51 errCnt = %u\n",errCnt);
#endif

	// Test udivsi3
	if(__udivsi3(4294967295UL,16777216UL) != 255UL)
		errCnt++;

	// Test udivsi4
	if(__udivsi4(4294967295UL,16777216UL) != 255UL)
		errCnt++;

	// Test umodsi3
	if(__umodsi3(4294967295UL,16777216UL) != 16777215UL)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE52 errCnt = %u\n",errCnt);
#endif

	// Test setjmp and longjmp
	long_jmp_var = 0;

	if(setjmp(env) == 0)
	{
		fcnt_long_jmp1();
	}
	else
	{
		if(long_jmp_var != 1)
			errCnt++;
	}

	if(long_jmp_var != 1)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE53 errCnt = %u\n",errCnt);
#endif

	// Test ffs
	if(ffs(0x0F10) != 5) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE54 errCnt = %u\n",errCnt);
#endif

	// Test strcasecmp, strncasecmp
	if(strcasecmp("ALPHA","alpha")) errCnt++;
	if(!strcasecmp("BETA","alpha")) errCnt++;
	if(strncasecmp("ALPHABETA","alphagamma",5)) errCnt++;
	if(!strncasecmp("ALPHABETA","alphagamma",6)) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE55 errCnt = %u\n",errCnt);
#endif

	// Test bcmp
	static const BYTE array1[] = {0x01,0x02,0x03,0x04};
	static const BYTE array2[] = {0x01,0x02,0x03,0x04};
	static const BYTE array3[] = {0x04,0x03,0x02,0x01};

	if(bcmp(array1,array2,sizeof(array1))) errCnt++;
	if(!bcmp(array1,array3,sizeof(array1))) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE56 errCnt = %u\n",errCnt);
#endif

	// Test bcopy
	static BYTE array4[4];

	bcopy(array1,array4,sizeof(array1));
	if(bcmp(array1,array4,sizeof(array1))) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE57 errCnt = %u\n",errCnt);
#endif

	// Test bzero
	const BYTE array_zero[] = {0x0,0x0,0x0,0x0};
	bzero(array4,sizeof(array4));
	if(bcmp(array4,array_zero,sizeof(array_zero))) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE58 errCnt = %u\n",errCnt);
#endif

	// Test index, rindex
	char *car_ptr = index(LOVE_STRING,'J');
	if(*car_ptr != 'J') errCnt++;
	if(index(LOVE_STRING,'Z') != NULL) errCnt++;
	car_ptr = rindex(LOVE_STRING,'A');
	if(*car_ptr != 'A') errCnt++;
	if(rindex(LOVE_STRING,'Z') != NULL) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE59 errCnt = %u\n",errCnt);
#endif

	// Test memchr
	BYTE *byte_ptr = memchr(array1,0x02,sizeof(array1));
	if(*byte_ptr != 0x02) errCnt++;
	if(memchr(array1,0x05,sizeof(array1)) != NULL) errCnt++;

	// Test memcmp
	if(memcmp(array1,array2,sizeof(array1))) errCnt++;
	if(!memcmp(array1,array3,sizeof(array1))) errCnt++;

	// Test memcpy
	if(memcpy(array4,array1,sizeof(array1)) != array4) errCnt++;
	if(memcmp(array1,array4,sizeof(array1))) errCnt++;

	// Test memmove
	if(memmove(&array4[1],array4,sizeof(array4)) != &array4[1]) errCnt++;
	if(array4[0] != 0x01 || array4[1] != 0x01 || array4[2] != 0x02 || array4[3] != 0x03) errCnt++;

	// Test memset
	if(memset(array4,0x04,sizeof(array4)) != array4) errCnt++;
	if(array4[0] != 0x04 || array4[1] != 0x04 || array4[2] != 0x04 || array4[3] != 0x04) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE60 errCnt = %u\n",errCnt);
#endif

	// Test strcpy, strcat, strncmp, strcmp, strlen, strcoll 
	if(strcpy(test_strings,TEST_STRING_1) != test_strings) errCnt++;
	if(strcat(test_strings,TEST_STRING_2) != test_strings) errCnt++;

	if(strncmp(test_strings,TEST_STRING_1_2,strlen(TEST_STRING_1_2)))
		errCnt++;

	if(strcmp(test_strings,TEST_STRING_1_2))
		errCnt++;
 
	if(strcoll(test_strings,TEST_STRING_1_2))
		errCnt++;

	// Test strchr, strrchr
	car_ptr = strchr(LOVE_STRING,'J');
	if(*car_ptr != 'J') errCnt++;
	if(strchr(LOVE_STRING,'Z') != NULL) errCnt++;
	car_ptr = strrchr(LOVE_STRING,'A');
	if(*car_ptr != 'A') errCnt++;
	if(strrchr(LOVE_STRING,'Z') != NULL) errCnt++;

	// Test strcspn, strspn
	if(strcspn("Amiga007","1234567890") != 5) errCnt++;
	if(strspn("amiga007","abcdefghijklmnopqrstuvwxyz") != 5) errCnt++;

	// Test strdup
	char *dup_str = strdup(LOVE_STRING);

	if(dup_str == NULL)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(dup_str,LOVE_STRING))
			errCnt++;

		free(dup_str);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE61 errCnt = %u\n",errCnt);
#endif

	// Test strerror, strerror_r
	if(!strncmp(strerror(1),UNKNOWN_ERROR,strlen(UNKNOWN_ERROR))) errCnt++;

	memset(buffer1,0,BUFFER_SIZE);

	if(strerror_r(1,buffer1,BUFFER_SIZE) != 0)
	{
		errCnt++;
	}
	else
	{
		if(!strncmp(buffer1,UNKNOWN_ERROR,strlen(UNKNOWN_ERROR)))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE62 errCnt = %u\n",errCnt);
#endif

	// Test strlcpy, strlcat
	strlcpy(buffer1,TEST_STRING_1,BUFFER_SIZE);
	strlcat(buffer1,TEST_STRING_2,BUFFER_SIZE);

	if(strcmp(buffer1,TEST_STRING_1_2))
		errCnt++;

	// Test strncpy, strncat
	strncpy(buffer1,TEST_STRING_1,strlen(TEST_STRING_1) + 1);
	strncat(buffer1,TEST_STRING_2,strlen(TEST_STRING_2) + 1);

	if(strcmp(buffer1,TEST_STRING_1_2))
		errCnt++;

	// Test strpbrk, strstr
	strcpy(buffer1,LOVE_STRING);
	if(strncmp(strpbrk(AMIGA_STRING,LOVE_STRING),AMIGA_STRING,strlen(AMIGA_STRING))) errCnt++;
	if(strncmp(strstr(LOVE_STRING,AMIGA_STRING),AMIGA_STRING,strlen(AMIGA_STRING))) errCnt++;

	// Test strtok
	WORD cnt_strtok = 0;
	char str_strtok[] = "- Some word with separators! -"; // this string will be modified!
	const char * separators = " ,.-!";
	char *strToken = strtok(str_strtok,separators);

    while(strToken != NULL)
	{
        strToken = strtok(NULL,separators);
		cnt_strtok++;
    }

	if(cnt_strtok != 4)
		errCnt++;

	// Test strtok_r
	cnt_strtok = 0;
	char str_strtok_r[] = "- Some word with separators! -"; // this string will be modified!
	char *saveptr;
	strToken = strtok_r(str_strtok_r,separators,&saveptr);

    while(strToken != NULL)
	{
        strToken = strtok_r(NULL,separators,&saveptr);
		cnt_strtok++;
    }

	if(cnt_strtok != 4)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE64 errCnt = %u\n",errCnt);
#endif

	// Test strxfrm
	memset(buffer1,0,BUFFER_SIZE);
    strxfrm(buffer1,LOVE_STRING,10);
	if(strncmp(buffer1,LOVE_STRING,10)) errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE65 errCnt = %u\n",errCnt);
#endif

	// Test tzset, ctime, ctime_r, gmtime, gmtime_r, asctime, localtime, clock, time
	time_t t = 1677762000UL; // typedef unsigned long time_t;
	struct tm tm;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	tzset(); // does nothing (just to verify linking)
	if(ctime(&t) == NULL) errCnt++;
	memset(buffer1,0,BUFFER_SIZE);
	if(ctime_r(&t,buffer1) != buffer1) errCnt++;
	if(gmtime(&t) == NULL) errCnt++;
	if(gmtime_r(&t,&tm) != &tm) errCnt++;
	if(strcmp(ctime(&t),asctime(localtime(&t)))) errCnt++;
	if(clock() == (clock_t)-1) errCnt++;
	if(time(&t) == (time_t)-1) errCnt++; // updates t with current time

#if OPTION_TRACE_CLIB
	printf("TRACE66 errCnt = %u\n",errCnt);
#endif

	// Test gettimeofday
	if(gettimeofday(&tv,NULL) != 0)
	{
		errCnt++;
	}
	else
	{
		if(tv.tv_sec == 0 && tv.tv_usec == 0)
			errCnt++;
	}

	// Test localtime_r, mktime	
	if(localtime_r(&t,&tm) != &tm)
	{
		errCnt++;
	}
	else
	{
		if(mktime(&tm) != t)
			errCnt++;
	}

	// Test strftime
	memset(buffer1,0,BUFFER_SIZE);

	if(strftime(buffer1,BUFFER_SIZE,"%Y",&tm) == 0)
		errCnt++;

	// Test asctime_r
	memset(buffer1,0,BUFFER_SIZE);

	if(asctime_r(localtime(&t),buffer1) != buffer1)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(ctime(&t),buffer1))
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE67 errCnt = %u\n",errCnt);
#endif

	// Test writev, readv
	struct iovec iov[2];

	// Write two strings in file
	strcpy(buffer1,TEST_STRING_1);
	strcpy(buffer2,TEST_STRING_2);
	iov[0].iov_base = buffer1;
	iov[0].iov_len = strlen(TEST_STRING_1);
	iov[1].iov_base = buffer2;
	iov[1].iov_len = strlen(TEST_STRING_2);

	if((file_desc = creat(RAM_TEST_FILE,S_IRWXU)) == ERROR) // mode is not used
	{
		errCnt++;
	}
	else
	{
		if((size_t)writev(file_desc,iov,2) != strlen(TEST_STRING_1_2))
			errCnt++;

		close(file_desc);
	}

	// Read the strings back
	iov[0].iov_base = buffer3;
	iov[0].iov_len = strlen(TEST_STRING_1);
	iov[1].iov_base = buffer4;
	iov[1].iov_len = strlen(TEST_STRING_2);

	if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((size_t)readv(file_desc,iov,2) != strlen(TEST_STRING_1_2))
			errCnt++;
		else
		{
			if(strncmp(buffer1,buffer3,strlen(TEST_STRING_1) || strncmp(buffer2,buffer4,strlen(TEST_STRING_2))))
				errCnt++;
		}

		close(file_desc);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE68 errCnt = %u\n",errCnt);
#endif

	// Test UL_GETFSIZE
	if(ulimit(UL_GETFSIZE,0L) < 0)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE69 errCnt = %u\n",errCnt);
#endif

	// Test unistd functions

	// Test access
	if(access(RAM_TEST_FILE,R_OK | W_OK) == ERROR)
		errCnt++;

	// Test chdir, getcwd
	if(chdir(RAM_PATH) == ERROR)
	{
		errCnt++;
	}
	else
	{
		memset(buffer1,0,BUFFER_SIZE);

		if(getcwd(buffer1,BUFFER_SIZE) != buffer1)
		{
			errCnt++;
		}
		else
		{
			if(strcmp(buffer1,RAM_DISK)) // RAM: becomes Ram Disk:
				errCnt++;
		}
	}

	// Test chown, lchown
	if(chown(RAM_TEST_FILE,(uid_t)-1,(gid_t)-1) == ERROR)
	{
		errCnt++;
	}

	if(lchown(RAM_TEST_FILE,(uid_t)-1,(gid_t)-1) == ERROR)
	{
		errCnt++;
	}

	// Test dup
	int new_file_desc,dup_file_desc;

	if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((dup_file_desc = dup(file_desc)) == ERROR)
		{
			errCnt++;
		}
		else
		{
			if((new_file_desc = open(NEW_RAM_TEST_FILE,O_RDONLY)) == ERROR)
			{
				errCnt++;
			}
			else
			{
				if(dup2(new_file_desc,dup_file_desc) != dup_file_desc) // old dup_file_desc is closed
				{
					errCnt++;
				}
				else
				{
					close(dup_file_desc);
				}

				close(new_file_desc);
			}
		}

		close(file_desc);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE70 errCnt = %u\n",errCnt);
#endif

	// Test fchown, fdatasync
	if((file_desc = open(RAM_TEST_FILE,O_RDWR)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(fdatasync(file_desc) == ERROR)
			errCnt++;

		if(fchown(file_desc,(uid_t)-1,(gid_t)-1) == ERROR)
			errCnt++;

		close(file_desc);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE71 errCnt = %u\n",errCnt);
#endif

	// Test fdopen, fileno, fsync
	FILE *fp;

	if((file_desc = open(RAM_TEST_FILE,O_WRONLY | O_CREAT | O_TRUNC)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((fp = fdopen(file_desc,"w")) == NULL)
		{
			errCnt++;
		}
		else
		{
 			if(fprintf(fp,LOVE_STRING) < 0)
				errCnt++;

			if(fsync(file_desc) == ERROR)
				errCnt++;

			if(fileno(fp) != file_desc)
				errCnt++;

 			fclose(fp);

			if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
			{
				errCnt++;
			}
			else
			{
				memset(buffer1,0,BUFFER_SIZE);

				if(read(file_desc,buffer1,BUFFER_SIZE) != (ssize_t)strlen(LOVE_STRING))
				{
					errCnt++;
				}
				else
				{
					if(strncmp(buffer1,LOVE_STRING,strlen(LOVE_STRING)))
					{
						errCnt++;
					}
				}

				close(file_desc);
			}
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE72 errCnt = %u\n",errCnt);
#endif

	// Test ftruncate
	if((file_desc = open(RAM_TEST_FILE,O_RDWR)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(ftruncate(file_desc,TRUNCATE_LENGTH) == ERROR)
			errCnt++;

		close(file_desc);

		if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
		{
			errCnt++;
		}
		else
		{
			memset(buffer1,0,BUFFER_SIZE);

			if(read(file_desc,buffer1,BUFFER_SIZE) < 0)
			{
				errCnt++;
			}
			else
			{
				if(strlen(buffer1) != TRUNCATE_LENGTH)
					errCnt++;
			}

			close(file_desc);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE73 errCnt = %u\n",errCnt);
#endif

	// Test getopt
	int opt;
	int argci = 4;
	char *argvi[] = {"cmd","-f","-i","-n"};
	int argCnt = 0;
      
	while((opt = getopt(argci,argvi,"ifn")) != -1) 
	{ 
		switch(opt) 
		{ 
			case 'i': 
			case 'f': 
			case 'n':
				argCnt++;
				break; 
		} 
	}

	if(argCnt != 3)
		errCnt++;

	// Test getpid
	if(getpid() == (pid_t)0)
		errCnt++;

	// Test isatty
	if((file_desc = open(RAM_TEST_FILE,O_RDWR)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(isatty(file_desc) != 0)
			errCnt++;

		close(file_desc);
	}

#if OPTION_TRACE_CLIB
	printf("TRACE74 errCnt = %u\n",errCnt);
#endif

	// Test link
	if(link(RAM_TEST_FILE,RAM_TEST_HARD_LINK) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((file_desc = open(RAM_TEST_HARD_LINK,O_RDONLY)) == ERROR)
		{
			errCnt++;
		}
		else
		{
			memset(buffer1,0,BUFFER_SIZE);

			if(read(file_desc,buffer1,BUFFER_SIZE) < 0)
			{
				errCnt++;
			}
			else
			{
				if(strlen(buffer1) != TRUNCATE_LENGTH)
					errCnt++;
			}

			close(file_desc);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE75 errCnt = %u\n",errCnt);
#endif

	// Test realpath
	memset(buffer1,0,BUFFER_SIZE);

	if(realpath(RAM_TEST_HARD_LINK,buffer1) != buffer1)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(buffer1,RAM_DISK_TEST_FILE)) // RAM: becomes Ram Disk:
			errCnt++;
	}

	// Test unlink
	if(unlink(RAM_TEST_HARD_LINK) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((file_desc = open(RAM_TEST_HARD_LINK,O_RDONLY)) != ERROR)
			errCnt++;
	}

#if OPTION_TRACE_CLIB
	printf("TRACE76 errCnt = %u\n",errCnt);
#endif

	// Test symlink, readlink (soft links are not implemented in 3.1)
#if 0 
	if((file_desc = creat(DH0_TEST_FILE,S_IRWXU)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((size_t)write(file_desc,LOVE_STRING,strlen(LOVE_STRING)) != strlen(LOVE_STRING))
		{
			errCnt++;
		}
		else
		{
			close(file_desc);
			unlink(DH0_TEST_SOFT_LINK); // delete any previous link

			if(symlink(DH0_TEST_FILE,DH0_TEST_SOFT_LINK) == ERROR)
			{
				errCnt++;
			}
			else
			{
				if((file_desc = open(DH0_TEST_SOFT_LINK,O_RDONLY)) == ERROR)
				{
					errCnt++;
				}
				else
				{
					memset(buffer1,0,BUFFER_SIZE);

					if(read(file_desc,buffer1,BUFFER_SIZE) < 0)
					{
						errCnt++;
					}
					else
					{
						if(strlen(buffer1) != strlen(LOVE_STRING))
							errCnt++;
					}

					close(file_desc);
				}

				memset(buffer1,0,BUFFER_SIZE);

				if(readlink(DH0_TEST_SOFT_LINK,buffer1,BUFFER_SIZE) == ERROR)
				{
					errCnt++;
				}
			}
		}
	}
#endif

	// Test lockf (in fcnlt_fcnlt.c, "#if defined(UNIX_PATH_SEMANTICS)" has been commented to get lockf working in clib)
	if((file_desc = open(RAM_TEST_FILE,O_RDWR)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(lockf(file_desc,F_LOCK,LOCKF_LEN) == ERROR)
		{
			errCnt++;
		}
		else
		{
			if(lockf(file_desc,F_ULOCK,LOCKF_LEN) == ERROR)
				errCnt++;

		}

		close(file_desc);
	}

	// Test sleep
	if(sleep(1) != 0)
		errCnt++;
	
	// Test usleep
	usleep(1000);

	// Test utime
	if(utime(RAM_TEST_FILE,NULL) == ERROR)
		errCnt++;

#if OPTION_TRACE_CLIB
	printf("TRACE77 errCnt = %u\n",errCnt);
#endif

	// Test uname
	struct utsname info;

	if(uname(&info) == ERROR)
		errCnt++;

	// Test truncate
	if(truncate(RAM_TEST_FILE,TRUNCATE_LENGTH) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
		{
			errCnt++;
		}
		else
		{
			memset(buffer1,0,BUFFER_SIZE);

			if(read(file_desc,buffer1,BUFFER_SIZE) < 0)
			{
				errCnt++;
			}
			else
			{
				if(strlen(buffer1) != TRUNCATE_LENGTH)
					errCnt++;
			}

			close(file_desc);
		}
	}

#if OPTION_TRACE_CLIB
	printf("TRACE78 errCnt = %u\n",errCnt);
#endif

	// Test remaining stdlib functions

	// Test abort (change the directive to 1 to test)
#if 0
	signal(SIGABRT,catch_SIGABRT);
	abort();
#endif

	// Test atexit, exit (change the directive to 1 to test)
#if 0
	atexit(exit_function);
	exit(0);
#endif

	return(errCnt);
}