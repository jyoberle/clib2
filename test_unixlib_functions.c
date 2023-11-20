// Checks the unixlib support functions
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
#include <getopt.h>
#include <sys/resource.h>
#include <sys/systeminfo.h>
#include <termios.h>
#include <sys/timeb.h>

#define RAM_PATH "RAM:"
#define RAM_DISK "/Ram Disk"
#define RAM_TEST_FILE "/RAM/test_file"
#define RAM_DISK_TEST_FILE "/Ram Disk/test_file"
#define NEW_RAM_TEST_FILE "/RAM/new_test_file"
#define RAM_TEST_HARD_LINK "/RAM/test_file_hard_link"
#define DH0_TEST_FILE "dh0:test_file"
#define DH0_TEST_SOFT_LINK "dh0:test_file_soft_link"
#define TEST_STRING_1 "In the beginning "
#define TEST_STRING_2 "God created the heaven and the Earth."
#define TEST_STRING_1_2 "In the beginning God created the heaven and the Earth."
#define DOS_TYPE "DOS"
#define RAM_TEST_DIR "/RAM/test_dir"
#define PIPE_CMD "C:echo "
#define PIPE_TEST_STR "pipe test"
#define BUFFER_SIZE 256
#define RAM_TEMP_FILE "/RAM/temp_file_XXXXXX"
#define RAM_TEMP_FILE_BIS "/RAM/temp_file_bis_XXXXXX"
#define RAM_TEMP_DIR "/RAM/temp_dir_XXXXXX"
#define LOVE_STRING "JOB loves Amiga"
#define TRUNCATE_LENGTH 3
#define LOCKF_LEN 4
#define OPTION_TRACE_UNIX 0

extern struct ExecBase *SysBase;
extern struct DosLibrary *DOSBase;

static char buffer1[BUFFER_SIZE],buffer2[BUFFER_SIZE],buffer3[BUFFER_SIZE],buffer4[BUFFER_SIZE],tempFileName[BUFFER_SIZE];
static WORD errCnt;

WORD testUnixlibFunctions(void)
{
	errCnt = 0;

	// Test dir functions (opendir, readdir, rewinddir, closedir)
	DIR *dir;
	struct dirent *dir_entry;

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

#if OPTION_TRACE_UNIX
	printf("TRACE1 errCnt = %u\n",errCnt);
#endif

	// Test fcntl functions (creat, open, fcntl)
	int file_desc;

	if((file_desc = creat(RAM_TEST_FILE,S_IRWXU)) == ERROR) // mode is not used
	{
		errCnt++;
	}
	else
	{
		if((size_t)write(file_desc,TEST_STRING_1,strlen(TEST_STRING_1)) != strlen(TEST_STRING_1))
		{
			errCnt++;
		}

		if(close(file_desc) == ERROR)
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
				if(fcntl(file_desc,F_GETFL) != 0)
				{
					errCnt++;
				}

				if(close(file_desc) == ERROR)
				{
					errCnt++;
				}
			}

		}
	}

#if OPTION_TRACE_UNIX
	printf("TRACE2 errCnt = %u\n",errCnt);
#endif

	// Test getopt_long
	int opt;
	char *argvi[] = {"cmd","-f","-i","-n","--title","Tropique","--artist","Dacq"};
	int argci = 8;
	int argCntShort = 0,argCntLong = 0;

	static struct option long_options[] =
	{
		{"title", required_argument, NULL, 't'},
		{"artist", required_argument, NULL, 'a'},
		{NULL, 0, NULL, 0}
	};
      
	while((opt = getopt_long(argci,argvi,"ifnta",&long_options,NULL)) != -1) 
	{ 
		switch(opt) 
		{ 
			case 'i': 
			case 'f': 
			case 'n':
				argCntShort++;
				break; 

			case 't':
			case 'a':
				argCntLong++;
				break;
		} 
	}

	if(argCntShort != 3 || argCntLong != 2)
		errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE3 errCnt = %u\n",errCnt);
#endif

	// Test fstatfs, statfs, lstat
	struct statfs stat_fs;
	struct stat file_stat;

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

	if(lstat(RAM_TEST_FILE,&file_stat) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(file_stat.st_mode & S_IXUSR == 0L)
			errCnt++;
	}

#if OPTION_TRACE_UNIX
	printf("TRACE4 errCnt = %u\n",errCnt);
#endif

	// Test getrlimit, setrlimit
	struct rlimit rlim;
	if(getrlimit(RLIM_CPU,&rlim) != 0) errCnt++;
	if(setrlimit(RLIM_CPU,&rlim) != -1) errCnt++; // nothing implemented

	// Test chmod, stat
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

	// Test mkdir, rmdir
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

#if OPTION_TRACE_UNIX
	printf("TRACE5 errCnt = %u\n",errCnt);
#endif

	// Test ctermid
	if(ctermid(NULL) == NULL) errCnt++;

	// Test fopen, fprintf, fflush, fclose, fread
	FILE *file_ptr;

	if((file_ptr = fopen(RAM_TEST_FILE,"w")) == NULL)
	{
		errCnt++;
	}
	else
	{
		if(fprintf(file_ptr,"%s%s",TEST_STRING_1,TEST_STRING_2) != (int)(strlen(TEST_STRING_1) + strlen(TEST_STRING_2)))
		{
			errCnt++;
		}
		else
		{
			if(fflush(file_ptr) != 0)
				errCnt++;

		}

		fclose(file_ptr);
	}

#if OPTION_TRACE_UNIX
	printf("TRACE6 errCnt = %u\n",errCnt);
#endif

	// Test popen (needs "cmdList":"C:Mount PIPE:" in launch.json)
	FILE *pipe;
	strcpy(buffer1,PIPE_CMD);
	strcat(buffer1,PIPE_TEST_STR);

	if((pipe = popen(buffer1,"r")) == NULL)
	{
		errCnt++;
	}
	else
	{
		memset(buffer2,0,BUFFER_SIZE);
		fread(buffer2,strlen(PIPE_TEST_STR),1,pipe);

		if(strncmp(buffer2,PIPE_TEST_STR,strlen(PIPE_TEST_STR)))
			errCnt++;

		pclose(pipe);
	}

#if OPTION_TRACE_UNIX
	printf("TRACE7 errCnt = %u\n",errCnt);
#endif

	// Test remove
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

#if OPTION_TRACE_UNIX
	printf("TRACE8 errCnt = %u\n",errCnt);
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

#if OPTION_TRACE_UNIX
	printf("TRACE9 errCnt = %u\n",errCnt);
#endif

	// Test alloca
	if(alloca(256) == NULL)
		errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE10 errCnt = %u\n",errCnt);
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

#if OPTION_TRACE_UNIX
	printf("TRACE11 errCnt = %u\n",errCnt);
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

#if OPTION_TRACE_UNIX
	printf("TRACE12 errCnt = %u\n",errCnt);
#endif

	// Test mktemp	
	strcpy(tempFileName,RAM_TEMP_FILE_BIS);

	if(strlen(mktemp(tempFileName)) == 0)
		errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE14 errCnt = %u\n",errCnt);
#endif

	// Test malloc, realloc
	void *memPtr;
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

#if OPTION_TRACE_UNIX
	printf("TRACE15 errCnt = %u\n",errCnt);
#endif

	// Test system
	printf("Displaying RAM: content with system...\n");

	if(system("dir RAM:") == -1)
		errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE16 errCnt = %u\n",errCnt);
#endif

	// Test sysinfo
	if(sysinfo(SI_SYSNAME,buffer1,BUFFER_SIZE) == -1) errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE17 errCnt = %u\n",errCnt);
#endif

	// Test tcgetattr, tcsetattr, cfsetispeed, cfsetospeed, cfgetispeed, cfgetospeed, tcdrain, tcflush, tcflow, tcsendbreak, cfmakeraw
	struct termios tios;

	if((file_desc = open("CON:",O_RDWR)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(tcgetattr(file_desc,&tios) == ERROR)
		{
			errCnt++;
		}
		else
		{
			if(cfsetispeed(&tios,B9600) == ERROR) errCnt++;
			if(cfsetospeed(&tios,B38400) == ERROR) errCnt++;
			if(cfgetispeed(&tios) != B9600) errCnt++;
			if(cfgetospeed(&tios) != B38400) errCnt++;
			if(cfmakeraw(&tios) == ERROR) errCnt++;

			if(tcsetattr(file_desc,TCSANOW,&tios) == ERROR) errCnt++;

			if(tcdrain(file_desc) == ERROR) errCnt++;
			if(tcflush(file_desc,TCOFLUSH) == ERROR) errCnt++;
			if(tcflow(file_desc,TCOOFF) == ERROR) errCnt++; // second argument is not used
			if(tcflow(file_desc,TCOON) == ERROR) errCnt++; // second argument is not used
			if(tcsendbreak(file_desc,1) == ERROR) errCnt++;
		}

		if(ttyname(file_desc) == NULL) errCnt++;
		if(ttyname_r(file_desc,buffer1,BUFFER_SIZE) != OK) errCnt++;

		close(file_desc);
	}

#if OPTION_TRACE_UNIX
	printf("TRACE18 errCnt = %u\n",errCnt);
#endif

	// Test ftime
	struct timeb tp;
	
	if(ftime(&tp) != 0)
		errCnt++;

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

#if OPTION_TRACE_UNIX
	printf("TRACE19 errCnt = %u\n",errCnt);
#endif

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

#if OPTION_TRACE_UNIX
	printf("TRACE20 errCnt = %u\n",errCnt);
#endif

	// Test access
	if(access(RAM_TEST_FILE,R_OK | W_OK) == ERROR)
		errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE21 errCnt = %u\n",errCnt);
#endif

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
			if(strcmp(buffer1,RAM_DISK)) // RAM: becomes /Ram Disk
				errCnt++;
		}
	}

#if OPTION_TRACE_UNIX
	printf("TRACE22 errCnt = %u\n",errCnt);
#endif

	// Test chown, lchown
	if(chown(RAM_TEST_FILE,(uid_t)-1,(gid_t)-1) == ERROR)
	{
		errCnt++;
	}

	if(lchown(RAM_TEST_FILE,(uid_t)-1,(gid_t)-1) == ERROR)
	{
		errCnt++;
	}

#if OPTION_TRACE_UNIX
	printf("TRACE23 errCnt = %u\n",errCnt);
#endif

	// Test execl, execle, execlp, execv, execve
	// All commands are commented out because they end with an exit()
	//char *exec[] = {"C:DIR","RAM:",(char *)NULL};
	//char *env[] = {"AMIGA=NUMBER_ONE",(char *)NULL};
	//if(execl("C:DIR","C:DIR","RAM:",(char *)NULL) == -1) errCnt++;
	//if(execle("C:DIR","C:DIR","RAM:",(char *)NULL,env) == -1) errCnt++;
	//if(execlp("C:DIR","C:DIR","RAM:",(char *)NULL) == -1) errCnt++;
	//if(execv("C:DIR",exec) == -1) errCnt++;
	//if(execve("C:DIR",exec,env) == -1) errCnt++;
	//if(execvp("C:DIR",exec) == -1) errCnt++;

	// Test fpathconf
	if((file_desc = open(RAM_TEST_FILE,O_RDONLY)) == ERROR)
	{
		errCnt++;
	}
	else
	{
		if(fpathconf(file_desc,_PC_PATH_MAX) == -1)
			errCnt++;
		
		close(file_desc);
	}

#if OPTION_TRACE_UNIX
	printf("TRACE24 errCnt = %u\n",errCnt);
#endif

	// Test lockf (only implemented when compiled with option UNIX_PATH_SEMANTICS)
	//#if UNIX_PATH_SEMANTICS
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
	//#endif

#if OPTION_TRACE_UNIX
	printf("TRACE25 errCnt = %u\n",errCnt);
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
			close(file_desc);
		}
	}

#if OPTION_TRACE_UNIX
	printf("TRACE26 errCnt = %u\n",errCnt);
#endif

	// Test realpath
	memset(buffer1,0,BUFFER_SIZE);

	if(realpath(RAM_TEST_HARD_LINK,buffer1) != buffer1)
	{
		errCnt++;
	}
	else
	{
		if(strcmp(buffer1,RAM_DISK_TEST_FILE))
			errCnt++;
	}

#if OPTION_TRACE_UNIX
	printf("TRACE27 errCnt = %u\n",errCnt);
#endif

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

#if OPTION_TRACE_UNIX
	printf("TRACE28 errCnt = %u\n",errCnt);
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

#if OPTION_TRACE_UNIX
	printf("TRACE29 errCnt = %u\n",errCnt);
#endif

	// Test utime
	if(utime(RAM_TEST_FILE,NULL) == ERROR)
		errCnt++;

#if OPTION_TRACE_UNIX
	printf("TRACE30 errCnt = %u\n",errCnt);
#endif

	return(errCnt);
}