# clib2 – An ISO 'C' (1994) compliant runtime library for AmigaOS (fork for use in vscode-amiga-debug)

This fork is based on the outstanding [clib2](https://github.com/adtools/clib2) written by Olaf Barthel. It is intented to be run in [vscode-amiga-debug](https://marketplace.visualstudio.com/items?itemName=JOB.amiga-debug-job) version 1.7.5 or higher (using gcc 13.2)

## Features

The below functions are provided:
- In `libc`: [isalnum](https://man7.org/linux/man-pages/man3/isalnum.3.html), [isalpha](https://man7.org/linux/man-pages/man3/isalpha.3.html), [isascii](https://man7.org/linux/man-pages/man3/isascii.3.html),
 [isblank](https://man7.org/linux/man-pages/man3/isblank.3.html), [iscntrl](https://man7.org/linux/man-pages/man3/iscntrl.3.html), [isdigit](https://man7.org/linux/man-pages/man3/isdigit.3.html),
 [isgraph](https://man7.org/linux/man-pages/man3/isgraph.3.html), [islower](https://man7.org/linux/man-pages/man3/islower.3.html), [isprint](https://man7.org/linux/man-pages/man3/isprint.3.html),
 [ispunct](https://man7.org/linux/man-pages/man3/ispunct.3.html), [isspace](https://man7.org/linux/man-pages/man3/isspace.3.html), [isupper](https://man7.org/linux/man-pages/man3/isupper.3.html),
 [isxdigit](https://man7.org/linux/man-pages/man3/isxdigit.3.html), [tolower](https://man7.org/linux/man-pages/man3/tolower.3.html), [toupper](https://man7.org/linux/man-pages/man3/toupper.3.html),
 [closedir](https://man7.org/linux/man-pages/man3/closedir.3.html), [opendir](https://man7.org/linux/man-pages/man3/opendir.3.html), [readdir](https://man7.org/linux/man-pages/man3/readdir.3.html),
 [rewinddir](https://man7.org/linux/man-pages/man3/rewinddir.3.html), [close](https://man7.org/linux/man-pages/man2/close.2.html), [creat](https://man7.org/linux/man-pages/man2/creat.2.html),
 [fcntl](https://man7.org/linux/man-pages/man2/fcntl.2.html), [lseek](https://man7.org/linux/man-pages/man2/lseek.2.html), [open](https://man7.org/linux/man-pages/man2/open.2.html),
 [read](https://man7.org/linux/man-pages/man2/read.2.html), [write](https://man7.org/linux/man-pages/man2/write.2.html), [ftw](https://man7.org/linux/man-pages/man3/ftw.3.html),
 [nftw](https://man7.org/linux/man-pages/man3/ftw.3.html), [imaxdiv](https://man7.org/linux/man-pages/man3/imaxdiv.3.html), [imaxabs](https://man7.org/linux/man-pages/man3/imaxabs.3.html), 
 [strtoimax](https://man7.org/linux/man-pages/man3/strtoimax.3.html), [strtoumax](https://man7.org/linux/man-pages/man3/strtoumax.3.html), [basename](https://man7.org/linux/man-pages/man3/basename.3.html),
 [dirname](https://man7.org/linux/man-pages/man3/dirname.3.html), [localeconv](https://man7.org/linux/man-pages/man3/localeconv.3.html), [setlocale](https://man7.org/linux/man-pages/man3/setlocale.3.html),
 [fstatfs](https://man7.org/linux/man-pages/man2/fstatfs.2.html), [statfs](https://man7.org/linux/man-pages/man2/statfs.2.html), [kill](https://man7.org/linux/man-pages/man2/kill.2.html),
 [raise](https://man7.org/linux/man-pages/man3/raise.3.html),  [sigaddset](https://man7.org/linux/man-pages/man3/sigsetops.3.html), [sigblock](https://man7.org/linux/man-pages/man3/sigvec.3.html),
 [sigemptyset](https://man7.org/linux/man-pages/man3/sigsetops.3.html), [sigmask](https://man7.org/linux/man-pages/man3/sigvec.3.html), [signal](https://man7.org/linux/man-pages/man2/signal.2.html),
 [sigprocmask](https://man7.org/linux/man-pages/man2/sigprocmask.2.html), [sigsetmask](https://man7.org/linux/man-pages/man3/sigvec.3.html), [chmod](https://man7.org/linux/man-pages/man2/chmod.2.html),
 [fchmod](https://man7.org/linux/man-pages/man2/fchmod.2.html), [fstat](https://man7.org/linux/man-pages/man2/fstat.2.html), [lstat](https://man7.org/linux/man-pages/man2/lstat.2.html),
 [mkdir](https://man7.org/linux/man-pages/man2/mkdir.2.html), [rmdir](https://man7.org/linux/man-pages/man2/rmdir.2.html), [stat](https://man7.org/linux/man-pages/man2/stat.2.html),
 [umask](https://man7.org/linux/man-pages/man2/umask.2.html), [asprintf](https://man7.org/linux/man-pages/man3/asprintf.3.html), [clearerr](https://man7.org/linux/man-pages/man3/clearerr.3.html),
 [fclose](https://man7.org/linux/man-pages/man3/fclose.3.html), [feof](https://man7.org/linux/man-pages/man3/feof.3.html), [ferror](https://man7.org/linux/man-pages/man3/ferror.3.html),
 [fflush](https://man7.org/linux/man-pages/man3/fflush.3.html), [fgetc](https://man7.org/linux/man-pages/man3/fgetc.3.html), [fgetpos](https://man7.org/linux/man-pages/man3/fgetpos.3.html),
 [fgets](https://man7.org/linux/man-pages/man3/fgets.3.html), [flockfile](https://man7.org/linux/man-pages/man3/flockfile.3.html),
 [fopen](https://man7.org/linux/man-pages/man3/fopen.3.html), [fprintf](https://man7.org/linux/man-pages/man3/fprintf.3.html), [fputc](https://man7.org/linux/man-pages/man3/fputc.3.html),
 [fread](https://man7.org/linux/man-pages/man3/fread.3.html), [freopen](https://man7.org/linux/man-pages/man3/freopen.3.html), [fscanf](https://man7.org/linux/man-pages/man3/fscanf.3.html),
 [fseek](https://man7.org/linux/man-pages/man3/fseek.3.html), [fseeko](https://man7.org/linux/man-pages/man3/fseeko.3.html), [fsetpos](https://man7.org/linux/man-pages/man3/fsetpos.3.html),
 [ftell](https://man7.org/linux/man-pages/man3/ftell.3.html), [ftello](https://man7.org/linux/man-pages/man3/ftello.3.html), [ftrylockfile](https://man7.org/linux/man-pages/man3/ftrylockfile.3.html),
 [funlockfile](https://man7.org/linux/man-pages/man3/funlockfile.3.html), [fwrite](https://man7.org/linux/man-pages/man3/fwrite.3.html), [getc](https://man7.org/linux/man-pages/man3/getc.3.html),
 [getc_unlocked](https://man7.org/linux/man-pages/man3/getc_unlocked.3.html), [getchar](https://man7.org/linux/man-pages/man3/getchar.3.html), [getchar_unlocked](https://man7.org/linux/man-pages/man3/getchar_unlocked.3.html),
 [gets](https://man7.org/linux/man-pages/man3/gets.3.html), [perror](https://man7.org/linux/man-pages/man3/perror.3.html),
 [popen](https://man7.org/linux/man-pages/man3/popen.3.html), [printf](https://man7.org/linux/man-pages/man3/printf.3.html), [putc](https://man7.org/linux/man-pages/man3/putc.3.html),
 [putc_unlocked](https://man7.org/linux/man-pages/man3/putc_unlocked.3.html), [putchar](https://man7.org/linux/man-pages/man3/putchar.3.html), [putchar_unlocked](https://man7.org/linux/man-pages/man3/putchar_unlocked.3.html),
 [puts](https://man7.org/linux/man-pages/man3/puts.3.html), [remove](https://man7.org/linux/man-pages/man3/remove.3.html), [rename](https://man7.org/linux/man-pages/man2/rename.2.html),
 [rewind](https://man7.org/linux/man-pages/man3/rewind.3.html), [scanf](https://man7.org/linux/man-pages/man3/scanf.3.html), [setbuf](https://man7.org/linux/man-pages/man3/setbuf.3.html),
 [setvbuf](https://man7.org/linux/man-pages/man3/setvbuf.3.html), [snprintf](https://man7.org/linux/man-pages/man3/snprintf.3.html), [sprintf](https://man7.org/linux/man-pages/man3/sprintf.3.html),
 [sscanf](https://man7.org/linux/man-pages/man3/sscanf.3.html), [tmpfile](https://man7.org/linux/man-pages/man3/tmpfile.3.html), [tmpnam](https://man7.org/linux/man-pages/man3/tmpnam.3.html),
 [ungetc](https://man7.org/linux/man-pages/man3/ungetc.3.html), [vasprintf](https://man7.org/linux/man-pages/man3/vasprintf.3.html), [vfprintf](https://man7.org/linux/man-pages/man3/vfprintf.3.html),
 [vfscanf](https://man7.org/linux/man-pages/man3/vfscanf.3.html), [vprintf](https://man7.org/linux/man-pages/man3/vprintf.3.html), [vsnprintf](https://man7.org/linux/man-pages/man3/vsnprintf.3.html),
 [vsprintf](https://man7.org/linux/man-pages/man3/vsprintf.3.html), [vsscanf](https://man7.org/linux/man-pages/man3/vsscanf.3.html), [vscanf](https://man7.org/linux/man-pages/man3/vscanf.3.html),
 [abort](https://man7.org/linux/man-pages/man3/abort.3.html), [abs](https://man7.org/linux/man-pages/man3/abs.3.html), [alloca](https://man7.org/linux/man-pages/man3/alloca.3.html),
 [atexit](https://man7.org/linux/man-pages/man3/atexit.3.html), [atoi](https://man7.org/linux/man-pages/man3/atoi.3.html), [atol](https://man7.org/linux/man-pages/man3/atol.3.html),
 [bsearch](https://man7.org/linux/man-pages/man3/bsearch.3.html), [calloc](https://man7.org/linux/man-pages/man3/calloc.3.html), [div](https://man7.org/linux/man-pages/man3/div.3.html),
 [__divsi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html), [exit](https://man7.org/linux/man-pages/man3/exit.3.html),
 [free](https://man7.org/linux/man-pages/man3/free.3.html), [getenv](https://man7.org/linux/man-pages/man3/getenv.3.html), [labs](https://man7.org/linux/man-pages/man3/labs.3.html),
 [llabs](https://man7.org/linux/man-pages/man3/llabs.3.html), [ldiv](https://man7.org/linux/man-pages/man3/ldiv.3.html), [malloc](https://man7.org/linux/man-pages/man3/malloc.3.html),
 [mkdtemp](https://man7.org/linux/man-pages/man3/mkdtemp.3.html), [mkstemp](https://man7.org/linux/man-pages/man3/mkstemp.3.html), [mktemp](https://man7.org/linux/man-pages/man3/mktemp.3.html),
 [__modsi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html), [__mulsi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html), [putenv](https://man7.org/linux/man-pages/man3/putenv.3.html),
 [qsort](https://man7.org/linux/man-pages/man3/qsort.3.html), [rand](https://man7.org/linux/man-pages/man3/rand.3.html), [rand_r](https://man7.org/linux/man-pages/man3/rand_r.3.html),
 [realloc](https://man7.org/linux/man-pages/man3/realloc.3.html), [setenv](https://man7.org/linux/man-pages/man3/setenv.3.html), [srand](https://man7.org/linux/man-pages/man3/srand.3.html),
 [strtol](https://man7.org/linux/man-pages/man3/strtol.3.html), [strtoul](https://man7.org/linux/man-pages/man3/strtoul.3.html), [system](https://man7.org/linux/man-pages/man3/system.3.html),
 [__udivsi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html), [__umodsi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html),
 [unsetenv](https://man7.org/linux/man-pages/man3/unsetenv.3.html), [ffs](https://man7.org/linux/man-pages/man3/ffs.3.html), [strcasecmp](https://man7.org/linux/man-pages/man3/strcasecmp.3.html),
 [strncasecmp](https://man7.org/linux/man-pages/man3/strncasecmp.3.html),
 [bcmp](https://man7.org/linux/man-pages/man3/bcmp.3.html), [bcopy](https://man7.org/linux/man-pages/man3/bcopy.3.html), [bzero](https://man7.org/linux/man-pages/man3/bzero.3.html),
 [index](https://man7.org/linux/man-pages/man3/index.3.html), [memchr](https://man7.org/linux/man-pages/man3/memchr.3.html), [memcmp](https://man7.org/linux/man-pages/man3/memcmp.3.html),
 [memcpy](https://man7.org/linux/man-pages/man3/memcpy.3.html), [memmove](https://man7.org/linux/man-pages/man3/memmove.3.html), [memset](https://man7.org/linux/man-pages/man3/memset.3.html),
 [rindex](https://man7.org/linux/man-pages/man3/rindex.3.html), [strcat](https://man7.org/linux/man-pages/man3/strcat.3.html), [strchr](https://man7.org/linux/man-pages/man3/strchr.3.html),
 [strcmp](https://man7.org/linux/man-pages/man3/strcmp.3.html), [strcoll](https://man7.org/linux/man-pages/man3/strcoll.3.html), [strcpy](https://man7.org/linux/man-pages/man3/strcpy.3.html),
 [strcspn](https://man7.org/linux/man-pages/man3/strcspn.3.html), [strdup](https://man7.org/linux/man-pages/man3/strdup.3.html), [strerror](https://man7.org/linux/man-pages/man3/strerror.3.html),
 [strerror_r](https://man7.org/linux/man-pages/man3/strerror_r.3.html), [strlcat](https://man7.org/linux/man-pages/man7/string_copying.7.html), [strlcpy](https://man7.org/linux/man-pages/man7/string_copying.7.html),
 [strlen](https://man7.org/linux/man-pages/man3/strlen.3.html), [strncat](https://man7.org/linux/man-pages/man3/strncat.3.html), [strncmp](https://man7.org/linux/man-pages/man3/strncmp.3.html),
 [strncpy](https://man7.org/linux/man-pages/man3/strncpy.3.html), [strpbrk](https://man7.org/linux/man-pages/man3/strpbrk.3.html), [strrchr](https://man7.org/linux/man-pages/man3/strrchr.3.html),
 [strspn](https://man7.org/linux/man-pages/man3/strspn.3.html), [strstr](https://man7.org/linux/man-pages/man3/strstr.3.html), [strtok](https://man7.org/linux/man-pages/man3/strtok.3.html),
 [strtok_r](https://man7.org/linux/man-pages/man3/strtok_r.3.html), [strxfrm](https://man7.org/linux/man-pages/man3/strxfrm.3.html), [asctime](https://man7.org/linux/man-pages/man3/asctime.3.html),
 [asctime_r](https://man7.org/linux/man-pages/man3/asctime_r.3.html), [clock](https://man7.org/linux/man-pages/man3/clock.3.html),
 [ctime](https://man7.org/linux/man-pages/man3/ctime.3.html), [ctime_r](https://man7.org/linux/man-pages/man3/ctime_r.3.html), [gettimeofday](https://man7.org/linux/man-pages/man2/gettimeofday.2.html),
 [gmtime](https://man7.org/linux/man-pages/man3/gmtime.3.html), [gmtime_r](https://man7.org/linux/man-pages/man3/gmtime_r.3.html), [localtime](https://man7.org/linux/man-pages/man3/localtime.3.html),
 [localtime_r](https://man7.org/linux/man-pages/man3/localtime_r.3.html), [mktime](https://man7.org/linux/man-pages/man3/mktime.3.html), [strftime](https://man7.org/linux/man-pages/man3/strftime.3.html),
 [time](https://man7.org/linux/man-pages/man2/time.2.html), [tzset](https://man7.org/linux/man-pages/man3/tzset.3.html), [readv](https://man7.org/linux/man-pages/man2/readv.2.html),
 [writev](https://man7.org/linux/man-pages/man2/writev.2.html),
 [ulimit](https://man7.org/linux/man-pages/man3/ulimit.3.html), [access](https://man7.org/linux/man-pages/man2/access.2.html), [chdir](https://man7.org/linux/man-pages/man2/chdir.2.html),
 [chown](https://man7.org/linux/man-pages/man2/chown.2.html), [dup](https://man7.org/linux/man-pages/man2/dup.2.html), [dup2](https://man7.org/linux/man-pages/man2/dup2.2.html),
 [fchown](https://man7.org/linux/man-pages/man2/fchown.2.html), [fdatasync](https://man7.org/linux/man-pages/man2/fdatasync.2.html), [fdopen](https://man7.org/linux/man-pages/man3/fdopen.3.html),
 [fileno](https://man7.org/linux/man-pages/man3/fileno.3.html), [fsync](https://man7.org/linux/man-pages/man2/fsync.2.html), [ftruncate](https://man7.org/linux/man-pages/man2/ftruncate.2.html),
 [getcwd](https://man7.org/linux/man-pages/man2/getcwd.2.html), [getopt](https://man7.org/linux/man-pages/man3/getopt.3.html), [getpid](https://man7.org/linux/man-pages/man2/getpid.2.html),
 [isatty](https://man7.org/linux/man-pages/man3/isatty.3.html), [lchown](https://man7.org/linux/man-pages/man2/lchown.2.html), [link](https://man7.org/linux/man-pages/man2/link.2.html),
 [lockf](https://man7.org/linux/man-pages/man3/lockf.3.html), [readlink](https://man7.org/linux/man-pages/man2/readlink.2.html), [realpath](https://man7.org/linux/man-pages/man3/realpath.3.html),
 [sleep](https://man7.org/linux/man-pages/man3/sleep.3.html), [symlink](https://man7.org/linux/man-pages/man2/symlink.2.html), [truncate](https://man7.org/linux/man-pages/man2/truncate.2.html),
 [unlink](https://man7.org/linux/man-pages/man2/unlink.2.html), [usleep](https://man7.org/linux/man-pages/man3/usleep.3.html), [utime](https://man7.org/linux/man-pages/man2/utime.2.html),
 [uname](https://man7.org/linux/man-pages/man2/uname.2.html), [setjmp](https://man7.org/linux/man-pages/man3/setjmp.3.html), [longjmp](https://man7.org/linux/man-pages/man3/longjmp.3.html)
- In `libunix`: [closedir](https://man7.org/linux/man-pages/man3/closedir.3.html), [rewinddir](https://man7.org/linux/man-pages/man3/rewinddir.3.html), [opendir](https://man7.org/linux/man-pages/man3/opendir.3.html),
 [readdir](https://man7.org/linux/man-pages/man3/readdir.3.html), [creat](https://man7.org/linux/man-pages/man2/creat.2.html), [fcntl](https://man7.org/linux/man-pages/man2/fcntl.2.html),
 [open](https://man7.org/linux/man-pages/man2/open.2.html), [getopt_long](https://man7.org/linux/man-pages/man3/getopt_long.3.html), [statfs](https://man7.org/linux/man-pages/man2/statfs.2.html),
 [getrlimit](https://man7.org/linux/man-pages/man2/getrlimit.2.html), [setrlimit](https://man7.org/linux/man-pages/man2/setrlimit.2.html), [chmod](https://man7.org/linux/man-pages/man2/chmod.2.html),
 [fstat](https://man7.org/linux/man-pages/man2/fstat.2.html), [lstat](https://man7.org/linux/man-pages/man2/lstat.2.html), [mkdir](https://man7.org/linux/man-pages/man2/mkdir.2.html),
 [rmdir](https://man7.org/linux/man-pages/man2/rmdir.2.html), [stat](https://man7.org/linux/man-pages/man2/stat.2.html), [ctermid](https://man7.org/linux/man-pages/man3/ctermid.3.html),
 [fflush](https://man7.org/linux/man-pages/man3/fflush.3.html), [fopen](https://man7.org/linux/man-pages/man3/fopen.3.html), [popen](https://man7.org/linux/man-pages/man3/popen.3.html),
 [remove](https://man7.org/linux/man-pages/man3/remove.3.html), [rename](https://man7.org/linux/man-pages/man2/rename.2.html), [alloca](https://man7.org/linux/man-pages/man3/alloca.3.html),
 [mkdtemp](https://man7.org/linux/man-pages/man3/mkdtemp.3.html), [mkstemp](https://man7.org/linux/man-pages/man3/mkstemp.3.html), [mktemp](https://man7.org/linux/man-pages/man3/mktemp.3.html),
 [malloc](https://man7.org/linux/man-pages/man3/malloc.3.html), [realloc](https://man7.org/linux/man-pages/man3/realloc.3.html), [system](https://man7.org/linux/man-pages/man3/system.3.html),
 [sysinfo](https://man7.org/linux/man-pages/man2/sysinfo.2.html), [cfgetispeed](https://man7.org/linux/man-pages/man3/cfgetispeed.3.html), [cfgetospeed](https://man7.org/linux/man-pages/man3/cfgetospeed.3.html),
 [cfmakeraw](https://man7.org/linux/man-pages/man3/cfmakeraw.3.html), [cfsetispeed](https://man7.org/linux/man-pages/man3/cfsetispeed.3.html), [cfsetospeed](https://man7.org/linux/man-pages/man3/cfsetospeed.3.html),
 [tcdrain](https://man7.org/linux/man-pages/man3/tcdrain.3.html), [tcflow](https://man7.org/linux/man-pages/man3/tcflow.3.html), [tcflush](https://man7.org/linux/man-pages/man3/tcflush.3.html),
 [tcgetattr](https://man7.org/linux/man-pages/man3/tcgetattr.3.html), [tcsendbreak](https://man7.org/linux/man-pages/man3/tcsendbreak.3.html), [tcsetattr](https://man7.org/linux/man-pages/man3/tcsetattr.3.html),
 [ftime](https://man7.org/linux/man-pages/man3/ftime.3.html), [readv](https://man7.org/linux/man-pages/man2/readv.2.html), [writev](https://man7.org/linux/man-pages/man2/writev.2.html),
 [access](https://man7.org/linux/man-pages/man2/access.2.html), [chdir](https://man7.org/linux/man-pages/man2/chdir.2.html), [chown](https://man7.org/linux/man-pages/man2/chown.2.html),
 [execl](https://man7.org/linux/man-pages/man3/execl.3.html), [execle](https://man7.org/linux/man-pages/man3/execle.3.html), [execlp](https://man7.org/linux/man-pages/man3/execlp.3.html),
 [execv](https://man7.org/linux/man-pages/man3/execv.3.html), [execve](https://man7.org/linux/man-pages/man2/execve.2.html), [execvp](https://man7.org/linux/man-pages/man3/execvp.3.html),
 [fpathconf](https://man7.org/linux/man-pages/man3/fpathconf.3.html), [getcwd](https://man7.org/linux/man-pages/man2/getcwd.2.html), [lchown](https://man7.org/linux/man-pages/man2/lchown.2.html),
 [link](https://man7.org/linux/man-pages/man2/link.2.html), [lockf](https://man7.org/linux/man-pages/man3/lockf.3.html), [pathconf](https://man7.org/linux/man-pages/man3/pathconf.3.html),
 [readlink](https://man7.org/linux/man-pages/man2/readlink.2.html), [realpath](https://man7.org/linux/man-pages/man3/realpath.3.html), [symlink](https://man7.org/linux/man-pages/man2/symlink.2.html),
 [truncate](https://man7.org/linux/man-pages/man2/truncate.2.html), [ttyname](https://man7.org/linux/man-pages/man3/ttyname.3.html), [ttyname_r](https://man7.org/linux/man-pages/man3/ttyname_r.3.html),
 [unlink](https://man7.org/linux/man-pages/man2/unlink.2.html), [utime](https://man7.org/linux/man-pages/man2/utime.2.html)
- In `libm`: [carg](https://man7.org/linux/man-pages/man3/carg.3.html), [cargf](https://man7.org/linux/man-pages/man3/cargf.3.html), [cargl](https://man7.org/linux/man-pages/man3/cargl.3.html),
 [cimag](https://man7.org/linux/man-pages/man3/cimag.3.html), [cimagf](https://man7.org/linux/man-pages/man3/cimagf.3.html), [cimagl](https://man7.org/linux/man-pages/man3/cimagl.3.html),
 [conj](https://man7.org/linux/man-pages/man3/conj.3.html), [conjf](https://man7.org/linux/man-pages/man3/conjf.3.html), [conjl](https://man7.org/linux/man-pages/man3/conjl.3.html),
 [creal](https://man7.org/linux/man-pages/man3/creal.3.html), [crealf](https://man7.org/linux/man-pages/man3/crealf.3.html), [creall](https://man7.org/linux/man-pages/man3/creall.3.html),
 [feclearexcept](https://man7.org/linux/man-pages/man3/feclearexcept.3.html), [fegetenv](https://man7.org/linux/man-pages/man3/fegetenv.3.html), [fegetexceptflag](https://man7.org/linux/man-pages/man3/fegetexceptflag.3.html),
 [fegetround](https://man7.org/linux/man-pages/man3/fegetround.3.html), [feholdexcept](https://man7.org/linux/man-pages/man3/feholdexcept.3.html), [feraiseexcept](https://man7.org/linux/man-pages/man3/feraiseexcept.3.html),
 [fesetenv](https://man7.org/linux/man-pages/man3/fesetenv.3.html), [fesetexceptflag](https://man7.org/linux/man-pages/man3/fesetexceptflag.3.html), [fetestexcept](https://man7.org/linux/man-pages/man3/fetestexcept.3.html),
 fetestround, [feupdateenv](https://man7.org/linux/man-pages/man3/feupdateenv.3.html), [acos](https://man7.org/linux/man-pages/man3/acos.3.html),
 [acosf](https://man7.org/linux/man-pages/man3/acosf.3.html), [acosh](https://man7.org/linux/man-pages/man3/acosh.3.html), [acoshf](https://man7.org/linux/man-pages/man3/acoshf.3.html),
 [asin](https://man7.org/linux/man-pages/man3/asin.3.html), [asinf](https://man7.org/linux/man-pages/man3/asinf.3.html), [asinh](https://man7.org/linux/man-pages/man3/asinh.3.html),
 [asinhf](https://man7.org/linux/man-pages/man3/asinhf.3.html), [atan](https://man7.org/linux/man-pages/man3/atan.3.html), [atan2](https://man7.org/linux/man-pages/man3/atan2.3.html),
 [atan2f](https://man7.org/linux/man-pages/man3/atan2f.3.html), [atanf](https://man7.org/linux/man-pages/man3/atanf.3.html), [atanh](https://man7.org/linux/man-pages/man3/atanh.3.html),
 [atanhf](https://man7.org/linux/man-pages/man3/atanhf.3.html), [cbrt](https://man7.org/linux/man-pages/man3/cbrt.3.html), [cbrtf](https://man7.org/linux/man-pages/man3/cbrtf.3.html),
 [ceil](https://man7.org/linux/man-pages/man3/ceil.3.html), [ceilf](https://man7.org/linux/man-pages/man3/ceilf.3.html), [copysign](https://man7.org/linux/man-pages/man3/copysign.3.html),
 [copysignf](https://man7.org/linux/man-pages/man3/copysignf.3.html), [cos](https://man7.org/linux/man-pages/man3/cos.3.html), [cosf](https://man7.org/linux/man-pages/man3/cosf.3.html),
 [cosh](https://man7.org/linux/man-pages/man3/cosh.3.html), [coshf](https://man7.org/linux/man-pages/man3/coshf.3.html), [erf](https://man7.org/linux/man-pages/man3/erf.3.html),
 [erfc](https://man7.org/linux/man-pages/man3/erfc.3.html), [erfcf](https://man7.org/linux/man-pages/man3/erfcf.3.html), [erff](https://man7.org/linux/man-pages/man3/erff.3.html),
 [exp](https://man7.org/linux/man-pages/man3/exp.3.html), [exp2](https://man7.org/linux/man-pages/man3/exp2.3.html), [exp2f](https://man7.org/linux/man-pages/man3/exp2f.3.html),
 [expf](https://man7.org/linux/man-pages/man3/expf.3.html), [expm1](https://man7.org/linux/man-pages/man3/expm1.3.html), [expm1f](https://man7.org/linux/man-pages/man3/expm1f.3.html),
 [fabs](https://man7.org/linux/man-pages/man3/fabs.3.html), [fabsf](https://man7.org/linux/man-pages/man3/fabsf.3.html), [fdim](https://man7.org/linux/man-pages/man3/fdim.3.html),
 [fdimf](https://man7.org/linux/man-pages/man3/fdimf.3.html), [floor](https://man7.org/linux/man-pages/man3/floor.3.html), [floorf](https://man7.org/linux/man-pages/man3/floorf.3.html),
 [fma](https://man7.org/linux/man-pages/man3/fma.3.html), [fmaf](https://man7.org/linux/man-pages/man3/fmaf.3.html), [fmax](https://man7.org/linux/man-pages/man3/fmax.3.html),
 [fmaxf](https://man7.org/linux/man-pages/man3/fmaxf.3.html), [fmin](https://man7.org/linux/man-pages/man3/fmin.3.html), [fminf](https://man7.org/linux/man-pages/man3/fminf.3.html),
 [fmod](https://man7.org/linux/man-pages/man3/fmod.3.html), [fmodf](https://man7.org/linux/man-pages/man3/fmodf.3.html), [frexp](https://man7.org/linux/man-pages/man3/frexp.3.html),
 [frexpf](https://man7.org/linux/man-pages/man3/frexpf.3.html), [hypot](https://man7.org/linux/man-pages/man3/hypot.3.html), [hypotf](https://man7.org/linux/man-pages/man3/hypotf.3.html),
 [ilogb](https://man7.org/linux/man-pages/man3/ilogb.3.html), [ilogbf](https://man7.org/linux/man-pages/man3/ilogbf.3.html),
 [ldexp](https://man7.org/linux/man-pages/man3/ldexp.3.html), [ldexpf](https://man7.org/linux/man-pages/man3/ldexpf.3.html),
 [lgamma](https://man7.org/linux/man-pages/man3/lgamma.3.html), [lgammaf](https://man7.org/linux/man-pages/man3/lgammaf.3.html), [llrint](https://man7.org/linux/man-pages/man3/llrint.3.html),
 [log](https://man7.org/linux/man-pages/man3/log.3.html), [log10](https://man7.org/linux/man-pages/man3/log10.3.html), [log10f](https://man7.org/linux/man-pages/man3/log10f.3.html),
 [log1p](https://man7.org/linux/man-pages/man3/log1p.3.html), [log1pf](https://man7.org/linux/man-pages/man3/log1pf.3.html), [log2](https://man7.org/linux/man-pages/man3/log2.3.html),
 [log2f](https://man7.org/linux/man-pages/man3/log2f.3.html), [logb](https://man7.org/linux/man-pages/man3/logb.3.html), [logbf](https://man7.org/linux/man-pages/man3/logbf.3.html),
 [logf](https://man7.org/linux/man-pages/man3/logf.3.html), [lrint](https://man7.org/linux/man-pages/man3/lrint.3.html), [lrintf](https://man7.org/linux/man-pages/man3/lrintf.3.html),
 [lround](https://man7.org/linux/man-pages/man3/lround.3.html), [lroundf](https://man7.org/linux/man-pages/man3/lroundf.3.html), [modf](https://man7.org/linux/man-pages/man3/modf.3.html),
 [modff](https://man7.org/linux/man-pages/man3/modff.3.html), [nan](https://man7.org/linux/man-pages/man3/nan.3.html), [nanf](https://man7.org/linux/man-pages/man3/nanf.3.html),
 [nearbyint](https://man7.org/linux/man-pages/man3/nearbyint.3.html), [nearbyintf](https://man7.org/linux/man-pages/man3/nearbyintf.3.html), [nextafter](https://man7.org/linux/man-pages/man3/nextafter.3.html),
 [nextafterf](https://man7.org/linux/man-pages/man3/nextafterf.3.html), [pow](https://man7.org/linux/man-pages/man3/pow.3.html) [powf](https://man7.org/linux/man-pages/man3/powf.3.html),
 [remainder](https://man7.org/linux/man-pages/man3/remainder.3.html), [remainderf](https://man7.org/linux/man-pages/man3/remainderf.3.html), [remquo](https://man7.org/linux/man-pages/man3/remquo.3.html),
 [remquof](https://man7.org/linux/man-pages/man3/remquof.3.html), [rint](https://man7.org/linux/man-pages/man3/rint.3.html), [rintf](https://man7.org/linux/man-pages/man3/rintf.3.html),
 [round](https://man7.org/linux/man-pages/man3/round.3.html), [roundf](https://man7.org/linux/man-pages/man3/roundf.3.html), [scalbn](https://man7.org/linux/man-pages/man3/scalbn.3.html),
 [scalbnf](https://man7.org/linux/man-pages/man3/scalbnf.3.html), [sin](https://man7.org/linux/man-pages/man3/sin.3.html), [sinf](https://man7.org/linux/man-pages/man3/sinf.3.html),
 [sinh](https://man7.org/linux/man-pages/man3/sinh.3.html), [sinhf](https://man7.org/linux/man-pages/man3/sinhf.3.html), [sqrt](https://man7.org/linux/man-pages/man3/sqrt.3.html),
 [sqrtf](https://man7.org/linux/man-pages/man3/sqrtf.3.html), [tan](https://man7.org/linux/man-pages/man3/tan.3.html), [tanf](https://man7.org/linux/man-pages/man3/tanf.3.html),
 [tanh](https://man7.org/linux/man-pages/man3/tanh.3.html), [tanhf](https://man7.org/linux/man-pages/man3/tanhf.3.html), [tgamma](https://man7.org/linux/man-pages/man3/tgamma.3.html),
 [tgammaf](https://man7.org/linux/man-pages/man3/tgammaf.3.html), [trunc](https://man7.org/linux/man-pages/man3/trunc.3.html), [truncf](https://man7.org/linux/man-pages/man3/truncf.3.html),
 [asprintf](https://man7.org/linux/man-pages/man3/asprintf.3.html), [fprintf](https://man7.org/linux/man-pages/man3/fprintf.3.html), [fscanf](https://man7.org/linux/man-pages/man3/fscanf.3.html),
 [printf](https://man7.org/linux/man-pages/man3/printf.3.html), [scanf](https://man7.org/linux/man-pages/man3/scanf.3.html), [snprintf](https://man7.org/linux/man-pages/man3/snprintf.3.html),
 [sprintf](https://man7.org/linux/man-pages/man3/sprintf.3.html), [sscanf](https://man7.org/linux/man-pages/man3/sscanf.3.html), [vasprintf](https://man7.org/linux/man-pages/man3/vasprintf.3.html),
 [vfprintf](https://man7.org/linux/man-pages/man3/vfprintf.3.html), [vfscanf](https://man7.org/linux/man-pages/man3/vfscanf.3.html), [vsscanf](https://man7.org/linux/man-pages/man3/vsscanf.3.html),
 [vscanf](https://man7.org/linux/man-pages/man3/vscanf.3.html), [vprintf](https://man7.org/linux/man-pages/man3/vprintf.3.html), [vsnprintf](https://man7.org/linux/man-pages/man3/vsnprintf.3.html),
 [vsprintf](https://man7.org/linux/man-pages/man3/vsprintf.3.html), [atof](https://man7.org/linux/man-pages/man3/atof.3.html), [strtod](https://man7.org/linux/man-pages/man3/strtod.3.html),
 [strtof](https://man7.org/linux/man-pages/man3/strtof.3.html), [difftime](https://man7.org/linux/man-pages/man3/difftime.3.html),
 [__adddf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__addsf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__divdf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__divsf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__eqdf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__eqsf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__extendsfdf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__fixdfsi](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__fixsfsi](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__fixunsdfsi](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__fixunssfsi](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__floatsidf](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__floatsisf](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__gedf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__gesf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__gtdf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__gtsf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__ledf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__lesf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__ltdf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__ltsf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__muldf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__mulsf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__nedf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__negdf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__negsf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__nesf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__subdf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__subsf3](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__truncdfsf2](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html),
 [__floatunsisf](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__floatunsidf](https://gcc.gnu.org/onlinedocs/gccint/Soft-float-library-routines.html), [__divdi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html),
 [__moddi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html), [__muldi3](https://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html)
 - In `libm881`: [carg](https://man7.org/linux/man-pages/man3/carg.3.html), [cargf](https://man7.org/linux/man-pages/man3/cargf.3.html), [cargl](https://man7.org/linux/man-pages/man3/cargl.3.html),
 [cimag](https://man7.org/linux/man-pages/man3/cimag.3.html), [cimagf](https://man7.org/linux/man-pages/man3/cimagf.3.html), [cimagl](https://man7.org/linux/man-pages/man3/cimagl.3.html),
 [conj](https://man7.org/linux/man-pages/man3/conj.3.html), [conjf](https://man7.org/linux/man-pages/man3/conjf.3.html), [conjl](https://man7.org/linux/man-pages/man3/conjl.3.html),
 [creal](https://man7.org/linux/man-pages/man3/creal.3.html), [crealf](https://man7.org/linux/man-pages/man3/crealf.3.html), [creall](https://man7.org/linux/man-pages/man3/creall.3.html),
 [feclearexcept](https://man7.org/linux/man-pages/man3/feclearexcept.3.html), [fegetenv](https://man7.org/linux/man-pages/man3/fegetenv.3.html), [fegetexceptflag](https://man7.org/linux/man-pages/man3/fegetexceptflag.3.html),
 [fegetround](https://man7.org/linux/man-pages/man3/fegetround.3.html), [feholdexcept](https://man7.org/linux/man-pages/man3/feholdexcept.3.html), [feraiseexcept](https://man7.org/linux/man-pages/man3/feraiseexcept.3.html),
 [fesetenv](https://man7.org/linux/man-pages/man3/fesetenv.3.html), [fesetexceptflag](https://man7.org/linux/man-pages/man3/fesetexceptflag.3.html), [fetestexcept](https://man7.org/linux/man-pages/man3/fetestexcept.3.html),
 fetestround, [feupdateenv](https://man7.org/linux/man-pages/man3/feupdateenv.3.html), [acos](https://man7.org/linux/man-pages/man3/acos.3.html),
 [acosf](https://man7.org/linux/man-pages/man3/acosf.3.html), [acosh](https://man7.org/linux/man-pages/man3/acosh.3.html), [acoshf](https://man7.org/linux/man-pages/man3/acoshf.3.html),
 [asin](https://man7.org/linux/man-pages/man3/asin.3.html), [asinf](https://man7.org/linux/man-pages/man3/asinf.3.html), [asinh](https://man7.org/linux/man-pages/man3/asinh.3.html),
 [asinhf](https://man7.org/linux/man-pages/man3/asinhf.3.html), [atan](https://man7.org/linux/man-pages/man3/atan.3.html), [atan2](https://man7.org/linux/man-pages/man3/atan2.3.html),
 [atan2f](https://man7.org/linux/man-pages/man3/atan2f.3.html), [atanf](https://man7.org/linux/man-pages/man3/atanf.3.html), [atanh](https://man7.org/linux/man-pages/man3/atanh.3.html),
 [atanhf](https://man7.org/linux/man-pages/man3/atanhf.3.html), [cbrt](https://man7.org/linux/man-pages/man3/cbrt.3.html), [cbrtf](https://man7.org/linux/man-pages/man3/cbrtf.3.html),
 [ceil](https://man7.org/linux/man-pages/man3/ceil.3.html), [ceilf](https://man7.org/linux/man-pages/man3/ceilf.3.html), [copysign](https://man7.org/linux/man-pages/man3/copysign.3.html),
 [copysignf](https://man7.org/linux/man-pages/man3/copysignf.3.html), [cos](https://man7.org/linux/man-pages/man3/cos.3.html), [cosf](https://man7.org/linux/man-pages/man3/cosf.3.html),
 [cosh](https://man7.org/linux/man-pages/man3/cosh.3.html), [coshf](https://man7.org/linux/man-pages/man3/coshf.3.html), [erf](https://man7.org/linux/man-pages/man3/erf.3.html),
 [erfc](https://man7.org/linux/man-pages/man3/erfc.3.html), [erfcf](https://man7.org/linux/man-pages/man3/erfcf.3.html), [erff](https://man7.org/linux/man-pages/man3/erff.3.html),
 [exp](https://man7.org/linux/man-pages/man3/exp.3.html), [exp2](https://man7.org/linux/man-pages/man3/exp2.3.html), [exp2f](https://man7.org/linux/man-pages/man3/exp2f.3.html),
 [expf](https://man7.org/linux/man-pages/man3/expf.3.html), [expm1](https://man7.org/linux/man-pages/man3/expm1.3.html), [expm1f](https://man7.org/linux/man-pages/man3/expm1f.3.html),
 [fabs](https://man7.org/linux/man-pages/man3/fabs.3.html), [fabsf](https://man7.org/linux/man-pages/man3/fabsf.3.html), [fdim](https://man7.org/linux/man-pages/man3/fdim.3.html),
 [fdimf](https://man7.org/linux/man-pages/man3/fdimf.3.html), [floor](https://man7.org/linux/man-pages/man3/floor.3.html), [floorf](https://man7.org/linux/man-pages/man3/floorf.3.html),
 [fma](https://man7.org/linux/man-pages/man3/fma.3.html), [fmaf](https://man7.org/linux/man-pages/man3/fmaf.3.html), [fmax](https://man7.org/linux/man-pages/man3/fmax.3.html),
 [fmaxf](https://man7.org/linux/man-pages/man3/fmaxf.3.html), [fmin](https://man7.org/linux/man-pages/man3/fmin.3.html), [fminf](https://man7.org/linux/man-pages/man3/fminf.3.html),
 [fmod](https://man7.org/linux/man-pages/man3/fmod.3.html), [fmodf](https://man7.org/linux/man-pages/man3/fmodf.3.html), [frexp](https://man7.org/linux/man-pages/man3/frexp.3.html),
 [frexpf](https://man7.org/linux/man-pages/man3/frexpf.3.html), [hypot](https://man7.org/linux/man-pages/man3/hypot.3.html), [hypotf](https://man7.org/linux/man-pages/man3/hypotf.3.html),
 [ilogb](https://man7.org/linux/man-pages/man3/ilogb.3.html), [ilogbf](https://man7.org/linux/man-pages/man3/ilogbf.3.html),
 [ldexp](https://man7.org/linux/man-pages/man3/ldexp.3.html), [ldexpf](https://man7.org/linux/man-pages/man3/ldexpf.3.html),
 [lgamma](https://man7.org/linux/man-pages/man3/lgamma.3.html), [lgammaf](https://man7.org/linux/man-pages/man3/lgammaf.3.html), [llrint](https://man7.org/linux/man-pages/man3/llrint.3.html),
 [log](https://man7.org/linux/man-pages/man3/log.3.html), [log10](https://man7.org/linux/man-pages/man3/log10.3.html), [log10f](https://man7.org/linux/man-pages/man3/log10f.3.html),
 [log1p](https://man7.org/linux/man-pages/man3/log1p.3.html), [log1pf](https://man7.org/linux/man-pages/man3/log1pf.3.html), [log2](https://man7.org/linux/man-pages/man3/log2.3.html),
 [log2f](https://man7.org/linux/man-pages/man3/log2f.3.html), [logb](https://man7.org/linux/man-pages/man3/logb.3.html), [logbf](https://man7.org/linux/man-pages/man3/logbf.3.html),
 [logf](https://man7.org/linux/man-pages/man3/logf.3.html), [lrint](https://man7.org/linux/man-pages/man3/lrint.3.html), [lrintf](https://man7.org/linux/man-pages/man3/lrintf.3.html),
 [lround](https://man7.org/linux/man-pages/man3/lround.3.html), [lroundf](https://man7.org/linux/man-pages/man3/lroundf.3.html), [modf](https://man7.org/linux/man-pages/man3/modf.3.html),
 [modff](https://man7.org/linux/man-pages/man3/modff.3.html), [nan](https://man7.org/linux/man-pages/man3/nan.3.html), [nanf](https://man7.org/linux/man-pages/man3/nanf.3.html),
 [nearbyint](https://man7.org/linux/man-pages/man3/nearbyint.3.html), [nearbyintf](https://man7.org/linux/man-pages/man3/nearbyintf.3.html), [nextafter](https://man7.org/linux/man-pages/man3/nextafter.3.html),
 [nextafterf](https://man7.org/linux/man-pages/man3/nextafterf.3.html), [pow](https://man7.org/linux/man-pages/man3/pow.3.html) [powf](https://man7.org/linux/man-pages/man3/powf.3.html),
 [remainder](https://man7.org/linux/man-pages/man3/remainder.3.html), [remainderf](https://man7.org/linux/man-pages/man3/remainderf.3.html), [remquo](https://man7.org/linux/man-pages/man3/remquo.3.html),
 [remquof](https://man7.org/linux/man-pages/man3/remquof.3.html), [rint](https://man7.org/linux/man-pages/man3/rint.3.html), [rintf](https://man7.org/linux/man-pages/man3/rintf.3.html),
 [round](https://man7.org/linux/man-pages/man3/round.3.html), [roundf](https://man7.org/linux/man-pages/man3/roundf.3.html), [scalbn](https://man7.org/linux/man-pages/man3/scalbn.3.html),
 [scalbnf](https://man7.org/linux/man-pages/man3/scalbnf.3.html), [sin](https://man7.org/linux/man-pages/man3/sin.3.html), [sinf](https://man7.org/linux/man-pages/man3/sinf.3.html),
 [sinh](https://man7.org/linux/man-pages/man3/sinh.3.html), [sinhf](https://man7.org/linux/man-pages/man3/sinhf.3.html), [sqrt](https://man7.org/linux/man-pages/man3/sqrt.3.html),
 [sqrtf](https://man7.org/linux/man-pages/man3/sqrtf.3.html), [tan](https://man7.org/linux/man-pages/man3/tan.3.html), [tanf](https://man7.org/linux/man-pages/man3/tanf.3.html),
 [tanh](https://man7.org/linux/man-pages/man3/tanh.3.html), [tanhf](https://man7.org/linux/man-pages/man3/tanhf.3.html), [tgamma](https://man7.org/linux/man-pages/man3/tgamma.3.html),
 [tgammaf](https://man7.org/linux/man-pages/man3/tgammaf.3.html), [trunc](https://man7.org/linux/man-pages/man3/trunc.3.html), [truncf](https://man7.org/linux/man-pages/man3/truncf.3.html),
 [asprintf](https://man7.org/linux/man-pages/man3/asprintf.3.html), [fprintf](https://man7.org/linux/man-pages/man3/fprintf.3.html), [fscanf](https://man7.org/linux/man-pages/man3/fscanf.3.html),
 [printf](https://man7.org/linux/man-pages/man3/printf.3.html), [scanf](https://man7.org/linux/man-pages/man3/scanf.3.html), [snprintf](https://man7.org/linux/man-pages/man3/snprintf.3.html),
 [sprintf](https://man7.org/linux/man-pages/man3/sprintf.3.html), [sscanf](https://man7.org/linux/man-pages/man3/sscanf.3.html), [vasprintf](https://man7.org/linux/man-pages/man3/vasprintf.3.html),
 [vfprintf](https://man7.org/linux/man-pages/man3/vfprintf.3.html), [vfscanf](https://man7.org/linux/man-pages/man3/vfscanf.3.html), [vsscanf](https://man7.org/linux/man-pages/man3/vsscanf.3.html),
 [vscanf](https://man7.org/linux/man-pages/man3/vscanf.3.html), [vprintf](https://man7.org/linux/man-pages/man3/vprintf.3.html), [vsnprintf](https://man7.org/linux/man-pages/man3/vsnprintf.3.html),
 [vsprintf](https://man7.org/linux/man-pages/man3/vsprintf.3.html), [atof](https://man7.org/linux/man-pages/man3/atof.3.html), [strtod](https://man7.org/linux/man-pages/man3/strtod.3.html),
 [strtof](https://man7.org/linux/man-pages/man3/strtof.3.html), [difftime](https://man7.org/linux/man-pages/man3/difftime.3.html)
 - In `libnet`: [accept](https://man7.org/linux/man-pages/man2/accept.2.html), [bind](https://man7.org/linux/man-pages/man2/bind.2.html), [connect](https://man7.org/linux/man-pages/man2/connect.2.html),
 [gethostbyaddr](https://man7.org/linux/man-pages/man3/gethostbyaddr.3.html), [gethostbyname](https://man7.org/linux/man-pages/man3/gethostbyname.3.html), [gethostid](https://man7.org/linux/man-pages/man3/gethostid.3.html),
 [gethostname](https://man7.org/linux/man-pages/man2/gethostname.2.html), [getnetbyaddr](https://man7.org/linux/man-pages/man3/getnetbyaddr.3.html), [getnetbyname](https://man7.org/linux/man-pages/man3/getnetbyname.3.html),
 [getpeername](https://man7.org/linux/man-pages/man2/getpeername.2.html), [getprotobyname](https://man7.org/linux/man-pages/man3/getprotobyname.3.html), [getprotobynumber](https://man7.org/linux/man-pages/man3/getprotobynumber.3.html),
 [getservbyname](https://man7.org/linux/man-pages/man3/getservbyname.3.html), [getservbyport](https://man7.org/linux/man-pages/man3/getservbyport.3.html), [getsockname](https://man7.org/linux/man-pages/man2/getsockname.2.html),
 [getsockopt](https://man7.org/linux/man-pages/man2/getsockopt.2.html), [inet_addr](https://man7.org/linux/man-pages/man3/inet_addr.3.html), [inet_aton](https://man7.org/linux/man-pages/man3/inet_aton.3.html),
 [inet_lnaof](https://man7.org/linux/man-pages/man3/inet_lnaof.3.html), [inet_makeaddr](https://man7.org/linux/man-pages/man3/inet_makeaddr.3.html), [inet_netof](https://man7.org/linux/man-pages/man3/inet_netof.3.html),
 [inet_network](https://man7.org/linux/man-pages/man3/inet_network.3.html), [inet_ntoa](https://man7.org/linux/man-pages/man3/inet_ntoa.3.html), [ioctl](https://man7.org/linux/man-pages/man2/ioctl.2.html),
 [listen](https://man7.org/linux/man-pages/man2/listen.2.html), [recv](https://man7.org/linux/man-pages/man2/recv.2.html), [recvfrom](https://man7.org/linux/man-pages/man2/recvfrom.2.html),
 [recvmsg](https://man7.org/linux/man-pages/man2/recvmsg.2.html), [select](https://man7.org/linux/man-pages/man2/select.2.html), waitselect,
 [send](https://man7.org/linux/man-pages/man2/send.2.html), [sendmsg](https://man7.org/linux/man-pages/man2/sendmsg.2.html), [sendto](https://man7.org/linux/man-pages/man2/sendto.2.html),
 [setsockopt](https://man7.org/linux/man-pages/man2/setsockopt.2.html), [shutdown](https://man7.org/linux/man-pages/man2/shutdown.2.html), [socket](https://man7.org/linux/man-pages/man2/socket.2.html),
 [hstrerror](https://man7.org/linux/man-pages/man3/hstrerror.3.html), [umask](https://man7.org/linux/man-pages/man2/umask.2.html), [sysinfo](https://man7.org/linux/man-pages/man2/sysinfo.2.html),
 [crypt](https://man7.org/linux/man-pages/man3/crypt.3.html), [endgrent](https://man7.org/linux/man-pages/man3/endgrent.3.html), [endpwent](https://man7.org/linux/man-pages/man3/endpwent.3.html),
 [getegid](https://man7.org/linux/man-pages/man2/getegid.2.html), [geteuid](https://man7.org/linux/man-pages/man2/geteuid.2.html), [getgid](https://man7.org/linux/man-pages/man2/getgid.2.html),
 [getgrent](https://man7.org/linux/man-pages/man3/getgrent.3.html), [getgrgid](https://man7.org/linux/man-pages/man3/getgrgid.3.html), [getgrnam](https://man7.org/linux/man-pages/man3/getgrnam.3.html),
 [getgroups](https://man7.org/linux/man-pages/man2/getgroups.2.html), [getpass](https://man7.org/linux/man-pages/man3/getpass.3.html), [getpwent](https://man7.org/linux/man-pages/man3/getpwent.3.html),
 [getpwnam](https://man7.org/linux/man-pages/man3/getpwnam.3.html), [getpwuid](https://man7.org/linux/man-pages/man3/getpwuid.3.html), [getuid](https://man7.org/linux/man-pages/man2/getuid.2.html),
 [initgroups](https://man7.org/linux/man-pages/man3/initgroups.3.html), [setegid](https://man7.org/linux/man-pages/man2/setegid.2.html), [seteuid](https://man7.org/linux/man-pages/man2/seteuid.2.html),
 [setgid](https://man7.org/linux/man-pages/man2/setgid.2.html), [setgrent](https://man7.org/linux/man-pages/man3/setgrent.3.html), [setgroups](https://man7.org/linux/man-pages/man2/setgroups.2.html),
 [setpwent](https://man7.org/linux/man-pages/man3/setpwent.3.html), [setregid](https://man7.org/linux/man-pages/man2/setregid.2.html), [setreuid](https://man7.org/linux/man-pages/man2/setreuid.2.html),
 [setsid](https://man7.org/linux/man-pages/man2/setsid.2.html), [setuid](https://man7.org/linux/man-pages/man2/setuid.2.html), [uname](https://man7.org/linux/man-pages/man2/uname.2.html)
 - In `libdebug`: [KCmpStr](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0172.html), [KDoFmt](https://d0.se/include/clib/debug_protos.h), [kgetc](https://d0.se/include/clib/debug_protos.h),
 [kgetch](https://d0.se/include/clib/debug_protos.h), [KGetCh](https://d0.se/include/clib/debug_protos.h), [kgetchar](https://d0.se/include/clib/debug_protos.h),
 [KGetChar](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0175.html), [kgetnum](https://d0.se/include/clib/debug_protos.h), [KGetNum](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0176.html),
 [KMayGetCh](https://d0.se/include/clib/debug_protos.h), [KMayGetChar](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0177.html), [kprintf](https://d0.se/include/clib/debug_protos.h),
 [KPrintF](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0178.html), [kputc](https://d0.se/include/clib/debug_protos.h), [kputch](https://d0.se/include/clib/debug_protos.h),
 [KPutCh](https://d0.se/include/clib/debug_protos.h), [kputchar](https://d0.se/include/clib/debug_protos.h), [KPutChar](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0179.html),
 [KPutFmt](https://d0.se/include/clib/debug_protos.h), [KPutS](https://d0.se/include/clib/debug_protos.h), [kputs](https://d0.se/include/clib/debug_protos.h),
 kputstr, [KPutStr](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node017A.html), kvprintf,
 [KVPrintF](https://d0.se/include/clib/debug_protos.h)
 - In `libamiga`: [ACrypt](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_3._guide/node001B.html), [AddTOF](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node013A.html), [ArgArrayDone](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node013C.html),
 [ArgArrayInit](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node013D.html), [ArgInt](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node013E.html), [ArgString](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node013F.html),
 [BeginIO](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0141.html), [CallHook](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0142.html), [CallHookA](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0143.html),
 [CoerceMethod](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0145.html), [CoerceMethodA](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0146.html), [CreateExtIO](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0147.html),
 [CreatePort](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0148.html), [CreateStdIO](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0149.html), [CreateTask](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node014A.html),
 [DeleteExtIO](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0152.html), [DeletePort](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0153.html), [DeleteStdIO](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0154.html),
 [DeleteTask](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0155.html), [DoMethod](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0156.html), [DoMethodA](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0157.html),
 [DoSuperMethod](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0158.html), [DoSuperMethodA](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0159.html), [DoTimer](https://d0.se/include/clib/alib_protos.h),
 [FastRand](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node015A.html), [afp](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node013B.html), [arnd](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0140.html),
 [dbf](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0151.html), [fpa](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node015B.html), [fpbcd](https://d0.se/include/clib/alib_protos.h),
 [FreeIEvents](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node015C.html), [HotKey](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node015F.html), [InvertString](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0160.html),
 [LibAllocPooled](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_3._guide/node0043.html), [LibCreatePool](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_3._guide/node0044.html), [LibDeletePool](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_3._guide/node0045.html),
 [LibFreePooled](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_3._guide/node0046.html), [NewList](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0161.html), [RangeRand](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0163.html),
 [RemTOF](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0164.html), [CheckRexxMsg](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0144.html), [GetRexxVar](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node015D.html),
 [SetRexxVar](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0165.html), [SetSuperAttrs](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0166.html),
 [TimeDelay](http://amigadev.elowar.com/read/ADCD_2.1/Includes_and_Autodocs_2._guide/node0169.html), [waitbeam](https://d0.se/include/clib/alib_protos.h)
 	
## How to create the libraries
- You need to have the `"ar"` command in your search path. This is normally the case for Linux and MacOS. For Windows, you can for example install [MSYS2](https://www.msys2.org/) / [MinGW](https://www.mingw-w64.org/) and update your search path to include their "bin" directory
- Install [_amiga-debug_ Visual Studio Code Extension (fork with libraries support)](https://marketplace.visualstudio.com/items?itemName=JOB.amiga-debug-job) version 1.7.5 or higher
- Choose the menu "File->Open Folder..." and select the directory of this github repository
- Open `.vscode/launch.json` and make `"kickstart"` point to your *Kickstart 3.1* ROM and `"workbench"` to your *Workbench 3.1* floppy (.adf)
- Hit <kbd>F5</kbd> to build the libraries (they wil appear in the `libs` directory, overwriting the existing ones)

## How to use the libraries
In order to test the libraries, you can uncomment in `main.c` the lines after "Printf("Testing clib2...\n");" and Hit <kbd>F5</kbd>. You will however have to follow some rules :
- `libc`, `libm`, `libnet` and `libamiga` can be tested together and the corresponding lines can be uncommented accordingly
- `libdebug` should be tested alone as you will have to follow the below specific procedure:\
a. The first time, once the program asks you to press Ctrl-C, do so\
b. Open UAE configuration window by pressing F12 and go to IO ports\
c. Under Serial Port, choose TCP://0.0.0.0:1234, select Direct and press OK\
d. Open a Telnet session on port 1234 by typing "telnet localhost 1234". For Windows, if needed, activate Telnet following the instructions on page https://www.makeuseof.com/enable-telnet-windows/.
 For MacOS, you can install it through Homebrew (see https://osxdaily.com/2018/07/18/get-telnet-macos/). For Linux, it should natively come with your distribution\
e. Go back to UAE and in the shell type a.exe to restart the program: you should see all chars and messages in the Telnet window. You will then be asked to type some chars and numbers in the Telnet window\
f. At the end, close the UAE window\
<u>Note</u>: after the test, UAE may signal a (false) read error on the Workench disk. This is due to the fact that we modified its configuration while running
- `libunix` should be tested after you changed the definition of the static libs order in the `Makefile` to put -lunix before -lc (see the commented static_libs lines in the `Makefile` and don't forget to put back the original lines after the test!)

If you want to use the libraries in your own project, your code will have to follow the skeleton of the included `main.c` file:
- Check for the Workbench startup message
- Assign to the variable __WBenchMsg the value of the pointer to the Workbench startup message, or assign NULL if started from command line
- Set __exit_blocked to FALSE to push exit() and similar functions to longjmp to target set by setjmp()
- Declare and open the following libraries: Exec, intuition.library, dos.library, utility.library, graphics.library, commodities.library and icon.library
- Assign to the variable __UtilityBase the value of UtilityBase
- Set the target `out:` (which is used by `exit`) through the `setjmp` instruction
- Call all the constructors, like in the `fcntCallCtor` function (the constructors must be called in a specific order)\
At the end:
- At the `out:` target, call the destructors, like in the `fcntCallDtor` function (the destructors must also be called in a specific order)
- Close all libraries
- If needed, reply to the Workbench startup message

It is also mandatory:
- To declare the static libs in your `Makefile`:\
a. static_libs := -lc -lm -ldebug -lnet -lunix -lc -lamiga, to use Amiga style paths\
b. static_libs := -lm -ldebug -lnet -lunix -lc -lamiga, to use UNIX style paths (e.g. "/RAM/myfile" instead of "RAM:myfile")
- To call SetPatch in your startup-sequence
- To increase the stack size, especially if you use `libunix` (see line below)
- To mount the PIPE: device if you intend to use pipe functions (popen, pclose, etc.); this is done in `.vscode/launch.json` file through the instruction "cmdList":"C:Mount PIPE:,C:stack 16384,C:SetPatch QUIET" (which also increases the stack size)
- To have the bsdsocket.library library installed in your system (it is required by `libnet`); this is done in `.vscode/launch.json` file through the instruction "bsdSocket":true
- To have at least the usergroup.library installed in your system (it is required by `libnet` for the usergroup functions); [vscode-amiga-debug](https://marketplace.visualstudio.com/items?itemName=JOB.amiga-debug-job) version 1.7.5 comes with the usergroup.library embedded in DH0:Libs
- To have AmiTCP installed in your system if you want to take full advantage of the usergroup functions (like `setuid`, `setgroups`, etc.)
- To *NOT* use `libnet` functions and native bsdsocket.library functions together, as they are incompatible; in order to avoid this, do not include "<inline/bsdsocket.h>" in your code if your intention is to use `libnet`

## Limitations
- `libm881` has not been tested and is provided as-is
- The function `hstrerror` is currently crashing the program when used
- ARexx functions `GetRexxVar` and `SetRexxVar` are not yet working in the current version of `libamiga`
- Neither 64 bits integers nor long double are supported
- The following functions are currently do-nothing: feclearexcept, fegetenv, fegetexceptflag, fegetround, feholdexcept, feraiseexcept, fesetenv, fesetexceptflag, fetestexcept, fetestround, feupdateenv

## Change Log (fork only)

### V1_214
- First version

## Additional credits

- Main code is coming from [clib2](https://github.com/adtools/clib2) written by Olaf Barthel.

- Code for ffp is coming from [MotoFFP](https://github.com/bayerf42/MotoFFP) and [Le_Lisp](https://github.com/GunterMueller/Le_Lisp/).

- Remaining code (additional functions, adaptations, wrappers) by [JOB](https://github.com/jyoberle).
