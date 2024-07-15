# to generate assembler listing with LTO, add to LDFLAGS: -Wa,-adhln=$@.listing,--listing-rhs-width=200
# for better annotations add -dA -dP
# to generate assembler source with LTO, add to LDFLAGS: -save-temps=cwd

ifdef OS
	WINDOWS = 1
	SHELL = cmd.exe
endif

TEST_PRG = \
	main.o \
	test_clib_functions.o \
	test_unixlib_functions.o \
	test_math_functions.o \
	test_netlib_functions.o \
	test_debuglib_functions.o \
	test_exec_support_functions.o \
	test_timer_support_functions.o \
	test_commodities_functions.o \
	test_ffp_functions.o \
	test_graphic_support_functions.o \
	test_hook_boopsi_functions.o \
	test_random_functions.o \
	test_mul_div_functions.o \
	test_arexx_functions.o \
	test_acrypt_function.o

C_LIB = \
	c.lib_rev.o \
	ctype_isalnum.o \
	ctype_isalpha.o \
	ctype_isascii.o \
	ctype_isblank.o \
	ctype_iscntrl.o \
	ctype_isdigit.o \
	ctype_isgraph.o \
	ctype_islower.o \
	ctype_isprint.o \
	ctype_ispunct.o \
	ctype_isspace.o \
	ctype_isupper.o \
	ctype_isxdigit.o \
	ctype_table.o \
	ctype_tolower.o \
	ctype_toupper.o \
	dirent_closedir.o \
	dirent_opendir.o \
	dirent_readdir.o \
	dirent_rewinddir.o \
	errno_data.o \
	fcntl_close.o \
	fcntl_creat.o \
	fcntl_fcntl.o \
	fcntl_get_default_file.o \
	fcntl_lock.o \
	fcntl_lseek.o \
	fcntl_open.o \
	fcntl_read.o \
	fcntl_write.o \
	ftw_ftw.o \
	ftw_nftw.o \
	inttypes_imaxdiv.o \
	inttypes_imaxabs.o \
	inttypes_strtoimax.o \
	inttypes_strtoumax.o \
	libgen_basename.o \
	libgen_dirname.o \
	locale_init_exit.o \
	locale_localeconv.o \
	locale_open_locale.o \
	locale_setlocale.o \
	mount_convertinfo.o \
	mount_fstatfs.o \
	mount_statfs.o \
	signal_checkabort.o \
	signal_data.o \
	signal_kill.o \
	signal_mask.o \
	signal_raise.o \
	signal_sigaddset.o \
	signal_sigblock.o \
	signal_sigemptyset.o \
	signal_sigmask.o \
	signal_signal.o \
	signal_sigprocmask.o \
	signal_sigsetmask.o \
	stat_chmod.o \
	stat_convertfileinfo.o \
	stat_fchmod.o \
	stat_fstat.o \
	stat_lstat.o \
	stat_lock.o \
	stat_mkdir.o \
	stat_rmdir.o \
	stat_stat.o \
	stat_umask.o \
	stdio_asprintf.o \
	stdio_change_fd_action.o \
	stdio_change_fd_user_data.o \
	stdio_clearerr.o \
	stdio_dropiobreadbuffer.o \
	stdio_duplicate_fd.o \
	stdio_examine_fh.o \
	stdio_fclose.o \
	stdio_fdhookentry.o \
	stdio_feof.o \
	stdio_ferror.o \
	stdio_fflush.o \
	stdio_fgetc.o \
	stdio_fgetpos.o \
	stdio_fgets.o \
	stdio_filliobreadbuffer.o \
	stdio_findvacantfdentry.o \
	stdio_findvacantiobentry.o \
	stdio_flockfile.o \
	stdio_flush.o \
	stdio_flush_all_files.o \
	stdio_flushiobwritebuffer.o \
	stdio_fopen.o \
	stdio_fprintf.o \
	stdio_fputc.o \
	stdio_fputs.o \
	stdio_fread.o \
	stdio_freopen.o \
	stdio_fscanf.o \
	stdio_fseek.o \
	stdio_fseeko.o \
	stdio_fsetpos.o \
	stdio_ftell.o \
	stdio_ftello.o \
	stdio_ftrylockfile.o \
	stdio_funlockfile.o \
	stdio_fwrite.o \
	stdio_getc.o \
	stdio_getc_unlocked.o \
	stdio_getchar.o \
	stdio_getchar_unlocked.o \
	stdio_gets.o \
	stdio_get_fd.o \
	stdio_get_file_descriptor.o \
	stdio_growfdtable.o \
	stdio_growiobtable.o \
	stdio_grow_file.o \
	stdio_initializefd.o \
	stdio_initializeiob.o \
	stdio_init_exit.o \
	stdio_file_init.o \
	stdio_iobhookentry.o \
	stdio_lock.o \
	stdio_locksemaphorename.o \
	stdio_nostdio.o \
	stdio_openiob.o \
	stdio_parent_of_fh.o \
	stdio_perror.o \
	stdio_popen.o \
	stdio_printf.o \
	stdio_putc.o \
	stdio_putc_unlocked.o \
	stdio_putchar.o \
	stdio_putchar_unlocked.o \
	stdio_puts.o \
	stdio_remove.o \
	stdio_remove_fd_alias.o \
	stdio_rename.o \
	stdio_resolve_fd_file.o \
	stdio_rewind.o \
	stdio_scanf.o \
	stdio_setbuf.o \
	stdio_setvbuf.o \
	stdio_snprintf.o \
	stdio_sprintf.o \
	stdio_sscanf.o \
	stdio_sscanf_hook_entry.o \
	stdio_tmpfile.o \
	stdio_tmpnam.o \
	stdio_translateioerror.o \
	stdio_ungetc.o \
	stdio_unlockfile.o \
	stdio_vasprintf.o \
	stdio_vasprintf_hook_entry.o \
	stdio_vfprintf.o \
	stdio_vfscanf.o \
	stdio_vprintf.o \
	stdio_vsnprintf.o \
	stdio_vsnprintf_hook_entry.o \
	stdio_vsprintf.o \
	stdio_vsprintf_hook_entry.o \
	stdio_vsscanf.o \
	stdio_vscanf.o \
	stdlib_abort.o \
	stdlib_abs.o \
	stdlib_alloca.o \
	stdlib_alloca_cleanup.o \
	stdlib_alloca_trap.o \
	stdlib_assertion_failure.o \
	stdlib_atexit.o \
	stdlib_atoi.o \
	stdlib_atol.o \
	stdlib_atoll.o \
	stdlib_bsearch.o \
	stdlib_calloc.o \
	stdlib_checkdetach.o \
	stdlib_constructor.o \
	stdlib_constructor_begin.o \
	stdlib_default_pool_size.o \
	stdlib_default_puddle_size.o \
	stdlib_destructor.o \
	stdlib_detach.o \
	stdlib_disablerequesters.o \
	stdlib_div.o \
	stdlib_divsi3.o \
	stdlib_divsi4.o \
	stdlib_dosbase.o \
	stdlib_exit.o \
	stdlib_free.o \
	stdlib_free_unused_slabs.o \
	stdlib_decay_unused_slabs.o \
	stdlib_getdefstacksize.o \
	stdlib_getenv.o \
	stdlib_getmemstats.o \
	stdlib_getsp.o \
	stdlib_get_errno.o \
	stdlib_get_slab_usage.o \
	stdlib_get_slab_allocations.o \
	stdlib_get_slab_stats.o \
	stdlib_isresident.o \
	stdlib_labs.o \
	stdlib_llabs.o \
	stdlib_ldiv.o \
	stdlib_lldiv.o \
	stdlib_lib_main.o \
	stdlib_lib_startup.o \
	stdlib_machine_test.o \
	stdlib_main.o \
	stdlib_main_stub.o \
	stdlib_malloc.o \
	stdlib_math.o \
	stdlib_mkdtemp.o \
	stdlib_mkstemp.o \
	stdlib_mktemp.o \
	stdlib_modsi3.o \
	stdlib_mulsi3.o \
	stdlib_never_free.o \
	stdlib_osliberror.o \
	stdlib_oslibversion.o \
	stdlib_priority.o \
	stdlib_process_name.o \
	stdlib_program_name.o \
	stdlib_putenv.o \
	stdlib_qsort.o \
	stdlib_rand.o \
	stdlib_rand_r.o \
	stdlib_realloc.o \
	stdlib_red_black.o \
	stdlib_resetmemstats.o \
	stdlib_semaphore.o \
	stdlib_setenv.o \
	stdlib_set_errno.o \
	stdlib_set_process_window.o \
	stdlib_shell_escape.o \
	stdlib_showerror.o \
	stdlib_slab.o \
	stdlib_slab_max_size.o \
	stdlib_slab_purge_threshold.o \
	stdlib_srand.o \
	stdlib_stacksize.o \
	stdlib_stack_usage.o \
	stdlib_arg.o \
	stdlib_stdio_window_spec.o \
	stdlib_strtol.o \
	stdlib_strtoll.o \
	stdlib_strtoul.o \
	stdlib_strtoull.o \
	stdlib_sysbase.o \
	stdlib_system.o \
	stdlib_termination_message.o \
	stdlib_threshold.o \
	stdlib_utilitybase.o \
	stdlib_udivsi3.o \
	stdlib_udivsi4.o \
	stdlib_umodsi3.o \
	stdlib_unsetenv.o \
	stdlib_ctzsi2.o \
	stdlib_clzsi2.o \
	strings_ffs.o \
	strings_strcasecmp.o \
	strings_strncasecmp.o \
	string_bcmp.o \
	string_bcopy.o \
	string_bzero.o \
	string_index.o \
	string_memchr.o \
	string_memcmp.o \
	string_memcpy.o \
	string_memmove.o \
	string_memset.o \
	string_rindex.o \
	string_strcat.o \
	string_strchr.o \
	string_strcmp.o \
	string_strcoll.o \
	string_strcpy.o \
	string_strcspn.o \
	string_strdup.o \
	string_strerror.o \
	string_strerror_r.o \
	string_strlcat.o \
	string_strlcpy.o \
	string_strlen.o \
	string_strncat.o \
	string_strncmp.o \
	string_strncpy.o \
	string_strpbrk.o \
	string_strrchr.o \
	string_strspn.o \
	string_strstr.o \
	string_strtok.o \
	string_strtok_r.o \
	string_strxfrm.o \
	time_asctime.o \
	time_asctime_r.o \
	time_clock.o \
	time_converttime.o \
	time_convert_datestamp.o \
	time_convert_time.o \
	time_ctime.o \
	time_ctime_r.o \
	time_data.o \
	time_days_per_date.o \
	time_gettimeofday.o \
	time_gmtime.o \
	time_gmtime_r.o \
	time_localtime.o \
	time_localtime_r.o \
	time_mktime.o \
	time_numbertostring.o \
	time_strftime.o \
	time_time.o \
	time_tzset.o \
	time_weekday.o \
	uio_readv.o \
	uio_writev.o \
	ulimit_ulimit.o \
	unistd_access.o \
	unistd_chdir.o \
	unistd_chdir_exit.o \
	unistd_chown.o \
	unistd_currentpathname.o \
	unistd_dup.o \
	unistd_dup2.o \
	unistd_fchown.o \
	unistd_fdatasync.o \
	unistd_fdopen.o \
	unistd_fileno.o \
	unistd_fsync.o \
	unistd_ftruncate.o \
	unistd_getcwd.o \
	unistd_getopt.o \
	unistd_getpid.o \
	unistd_init_exit.o \
	unistd_isatty.o \
	unistd_lchown.o \
	unistd_link.o \
	unistd_lockf.o \
	unistd_readlink.o \
	unistd_realpath.o \
	unistd_sleep.o \
	unistd_symlink.o \
	unistd_sync_fd.o \
	unistd_timer.o \
	unistd_time_delay.o \
	unistd_truncate.o \
	unistd_unlink.o \
	unistd_unlink_retries.o \
	unistd_usleep.o \
	utime_utime.o \
	utsname_uname.o

C_LIB_ASM = \
	stdlib_setjmp.o \
	stdlib_swapstack.o \
	ovbcopy.o

UNIX_LIB = \
	unix.lib_rev.o \
	dirent_closedir.o \
	dirent_rewinddir.o \
	dirent_opendir.o \
	dirent_readdir.o \
	fcntl_creat.o \
	fcntl_fcntl.o \
	fcntl_open.o \
	fcntl_get_default_file.o \
	getopt_getopt_long.o \
	mount_convertinfo.o \
	mount_statfs.o \
	resource_getrlimit.o \
	resource_setrlimit.o \
	stat_chmod.o \
	stat_fstat.o \
	stat_lstat.o \
	stat_lock.o \
	stat_mkdir.o \
	stat_rmdir.o \
	stat_stat.o \
	stdio_ctermid.o \
	stdio_fdhookentry.o \
	stdio_fflush.o \
	stdio_fopen.o \
	stdio_init_exit.o \
	stdio_file_init.o \
	stdio_locksemaphorename.o \
	stdio_openiob.o \
	stdio_popen.o \
	stdio_record_locking.o \
	stdio_remove.o \
	stdio_rename.o \
	stdlib_alloca.o \
	stdlib_alloca_cleanup.o \
	stdlib_alloca_trap.o \
	stdlib_arg.o \
	stdlib_expand_wildcard.o \
	stdlib_expand_wildcard_check.o \
	stdlib_getmemstats.o \
	stdlib_main.o \
	stdlib_main_stub.o \
	stdlib_mkdtemp.o \
	stdlib_mkstemp.o \
	stdlib_mktemp.o \
	stdlib_malloc.o \
	stdlib_realloc.o \
	stdlib_resetmemstats.o \
	stdlib_system.o \
	systeminfo_sysinfo.o \
	termios_cfgetispeed.o \
	termios_cfgetospeed.o \
	termios_cfmakeraw.o \
	termios_cfsetispeed.o \
	termios_cfsetospeed.o \
	termios_console_fdhookentry.o \
	termios_tcdrain.o \
	termios_tcflow.o \
	termios_tcflush.o \
	termios_tcgetattr.o \
	termios_tcsendbreak.o \
	termios_tcsetattr.o \
	timeb_ftime.o \
	uio_readv.o \
	uio_writev.o \
	unistd_access.o \
	unistd_chdir.o \
	unistd_chdir_exit.o \
	unistd_chown.o \
	unistd_common_pathconf.o \
	unistd_def_path.o \
	unistd_def_path_delimiter.o \
	unistd_environ.o \
	unistd_execl.o \
	unistd_execle.o \
	unistd_execlp.o \
	unistd_execv.o \
	unistd_execve.o \
	unistd_execve_env_exit.o \
	unistd_execve_env_init.o \
	unistd_execve_exit.o \
	unistd_execvp.o \
	unistd_fpathconf.o \
	unistd_getcwd.o \
	unistd_lchown.o \
	unistd_link.o \
	unistd_lockf.o \
	unistd_pathconf.o \
	unistd_readlink.o \
	unistd_realpath.o \
	unistd_restorepathname.o \
	unistd_setcurrentpath.o \
	unistd_strip_double_slash.o \
	unistd_symlink.o \
	unistd_translatea2u.o \
	unistd_translaterel.o \
	unistd_translateu2a.o \
	unistd_truncate.o \
	unistd_ttyname.o \
	unistd_ttyname_r.o \
	unistd_unix_path_semantics.o \
	unistd_unlink.o \
	unistd_wildcard_expand.o \
	utime_utime.o

MATH_LIB = \
	complex_carg.o \
	complex_cargf.o \
	complex_cargl.o \
	complex_cimag.o \
	complex_cimagf.o \
	complex_cimagl.o \
	complex_conj.o \
	complex_conjf.o \
	complex_conjl.o \
	complex_creal.o \
	complex_crealf.o \
	complex_creall.o \
	fenv_feclearexcept.o \
	fenv_fegetenv.o \
	fenv_fegetexceptflag.o \
	fenv_fegetround.o \
	fenv_feholdexcept.o \
	fenv_feraiseexcept.o \
	fenv_fesetenv.o \
	fenv_fesetexceptflag.o \
	fenv_fetestexcept.o \
	fenv_fetestround.o \
	fenv_feupdateenv.o \
	math_acos.o \
	math_acosf.o \
	math_acosh.o \
	math_acoshf.o \
	math_asin.o \
	math_asinf.o \
	math_asinh.o \
	math_asinhf.o \
	math_atan.o \
	math_atan2.o \
	math_atan2f.o \
	math_atanf.o \
	math_atanh.o \
	math_atanhf.o \
	math_cbrt.o \
	math_cbrtf.o \
	math_ceil.o \
	math_ceilf.o \
	math_copysign.o \
	math_copysignf.o \
	math_cos.o \
	math_cosf.o \
	math_cosh.o \
	math_coshf.o \
	math_erf.o \
	math_erfc.o \
	math_erfcf.o \
	math_erff.o \
	math_exp.o \
	math_exp2.o \
	math_exp2f.o \
	math_expf.o \
	math_expm1.o \
	math_expm1f.o \
	math_fabs.o \
	math_fabsf.o \
	math_fdim.o \
	math_fdimf.o \
	math_floor.o \
	math_floorf.o \
	math_fma.o \
	math_fmaf.o \
	math_fmax.o \
	math_fmaxf.o \
	math_fmin.o \
	math_fminf.o \
	math_fmod.o \
	math_fmodf.o \
	math_fpclassify.o \
	math_frexp.o \
	math_frexpf.o \
	math_huge_val.o \
	math_huge_valf.o \
	math_hypot.o \
	math_hypotf.o \
	math_ilogb.o \
	math_ilogbf.o \
	math_init_exit.o \
	math_isfinite.o \
	math_isunordered.o \
	math_kernel_cosf.o \
	math_kernel_sinf.o \
	math_kernel_tanf.o \
	math_ldexp.o \
	math_ldexpf.o \
	math_lgamma.o \
	math_lgammaf.o \
	math_llrint.o \
	math_log.o \
	math_log10.o \
	math_log10f.o \
	math_log1p.o \
	math_log1pf.o \
	math_log2.o \
	math_log2f.o \
	math_logb.o \
	math_logbf.o \
	math_logf.o \
	math_lrint.o \
	math_lrintf.o \
	math_lround.o \
	math_lroundf.o \
	math_modf.o \
	math_modff.o \
	math_nan.o \
	math_nanf.o \
	math_inf.o \
	math_inff.o \
	math_nearbyint.o \
	math_nearbyintf.o \
	math_nextafter.o \
	math_nextafterf.o \
	math_pow.o \
	math_powf.o \
	math_remainder.o \
	math_remainderf.o \
	math_remquo.o \
	math_remquof.o \
	math_rem_pio2f.o \
	math_rint.o \
	math_rintf.o \
	math_round.o \
	math_roundf.o \
	math_scalbn.o \
	math_scalbnf.o \
	math_signbit.o \
	math_sin.o \
	math_sinf.o \
	math_sinh.o \
	math_sinhf.o \
	math_sqrt.o \
	math_sqrtf.o \
	math_tan.o \
	math_tanf.o \
	math_tanh.o \
	math_tanhf.o \
	math_tgamma.o \
	math_tgammaf.o \
	math_trunc.o \
	math_truncf.o \
	stdio_asprintf.o \
	stdio_flush.o \
	stdio_flush_all_files.o \
	stdio_fprintf.o \
	stdio_fscanf.o \
	stdio_printf.o \
	stdio_scanf.o \
	stdio_snprintf.o \
	stdio_sprintf.o \
	stdio_sscanf.o \
	stdio_vasprintf.o \
	stdio_vfprintf.o \
	stdio_vfscanf.o \
	stdio_vsscanf.o \
	stdio_vscanf.o \
	stdio_vprintf.o \
	stdio_vsnprintf.o \
	stdio_vsprintf.o \
	stdlib_atof.o \
	stdlib_strtod.o \
	stdlib_strtof.o \
	time_difftime.o \
	math_kernel_sin.o \
	math_kernel_cos.o \

MATH_LIB_881 = \
	m881.lib_rev.o \
	$(MATH_LIB)

MATH_LIB_IEEE = \
	m.lib_rev.o \
	$(MATH_LIB) \
	math_adddf3.o \
	math_addsf3.o \
	math_divdf3.o \
	math_divsf3.o \
	math_eqdf2.o \
	math_eqsf2.o \
	math_extendsfdf2.o \
	math_fixdfsi.o \
	math_fixsfsi.o \
	math_fixunsdfsi.o \
	math_fixunssfsi.o \
	math_floatsidf.o \
	math_floatsisf.o \
	math_gedf2.o \
	math_gesf2.o \
	math_gtdf2.o \
	math_gtsf2.o \
	math_ledf2.o \
	math_lesf2.o \
	math_ltdf2.o \
	math_ltsf2.o \
	math_muldf3.o \
	math_mulsf3.o \
	math_nedf2.o \
	math_negdf2.o \
	math_negsf2.o \
	math_nesf2.o \
	math_subdf3.o \
	math_subsf3.o \
	math_truncdfsf2.o \
	math_floatunsisf.o \
	math_floatunsidf.o \
	math_divdi3.o \
	math_moddi3.o \
	math_muldi3.o \
	math_ashldi3.o \
	math_lshrdi3.o

NET_LIB = \
	net.lib_rev.o \
	socket_accept.o \
	socket_bind.o \
	socket_check_daemon.o \
	socket_connect.o \
	socket_gethostbyaddr.o \
	socket_gethostbyname.o \
	socket_gethostid.o \
	socket_gethostname.o \
	socket_getnetbyaddr.o \
	socket_getnetbyname.o \
	socket_getpeername.o \
	socket_getprotobyname.o \
	socket_getprotobynumber.o \
	socket_getservbyname.o \
	socket_getservbyport.o \
	socket_getsockname.o \
	socket_getsockopt.o \
	socket_get_descriptor.o \
	socket_inet_addr.o \
	socket_inet_aton.o \
	socket_inet_lnaof.o \
	socket_inet_makeaddr.o \
	socket_inet_netof.o \
	socket_inet_network.o \
	socket_inet_ntoa.o \
	socket_init_exit.o \
	socket_ioctl.o \
	socket_isdaemon.o \
	socket_listen.o \
	socket_obtain_daemon.o \
	socket_recv.o \
	socket_recvfrom.o \
	socket_recvmsg.o \
	socket_select_signal.o \
	socket_select.o \
	socket_wait_select.o \
	socket_send.o \
	socket_sendmsg.o \
	socket_sendto.o \
	socket_setsockopt.o \
	socket_shutdown.o \
	socket_socket.o \
	socket_hook_entry.o \
	socket_hstrerror.o \
	socket_get_h_errno.o \
	socket_set_h_errno.o \
	stat_umask.o \
	systeminfo_sysinfo.o \
	usergroup_crypt.o \
	usergroup_data.o \
	usergroup_endgrent.o \
	usergroup_endpwent.o \
	usergroup_getegid.o \
	usergroup_geteuid.o \
	usergroup_getgid.o \
	usergroup_getgrent.o \
	usergroup_getgrgid.o \
	usergroup_getgrnam.o \
	usergroup_getgroups.o \
	usergroup_getpass.o \
	usergroup_getpwent.o \
	usergroup_getpwnam.o \
	usergroup_getpwuid.o \
	usergroup_getuid.o \
	usergroup_initgroups.o \
	usergroup_init_exit.o \
	usergroup_rootmode.o \
	usergroup_setegid.o \
	usergroup_seteuid.o \
	usergroup_setgid.o \
	usergroup_setgrent.o \
	usergroup_setgroups.o \
	usergroup_setpwent.o \
	usergroup_setregid.o \
	usergroup_setreuid.o \
	usergroup_setsid.o \
	usergroup_setuid.o \
	utsname_uname.o \
	socket_getaddrinfo.o

DEBUG_LIB = \
	debug.lib_rev.o \
	debug.o \
	debug_cmpstrexec.o \
	debug_kcmpstr.o \
	debug_kdofmt.o \
	debug_kgetc.o \
	debug_kgetch1.o \
	debug_kgetch2.o \
	debug_kgetchar1.o \
	debug_kgetchar2.o \
	debug_kgetnum1.o \
	debug_kgetnum2.o \
	debug_kmaygetch.o \
	debug_kmaygetchar.o \
	debug_kprintf1.o \
	debug_kprintf2.o \
	debug_kputc.o \
	debug_kputch1.o \
	debug_kputch2.o \
	debug_kputchar1.o \
	debug_kputchar2.o \
	debug_kputfmt.o \
	debug_kputs1.o \
	debug_kputs2.o \
	debug_kputstr1.o \
	debug_kputstr2.o \
	debug_kvprintf1.o \
	debug_kvprintf2.o \
	debug_level.o

AMIGA_LIB = \
	amiga.lib_rev.o \
	amiga_acrypt.o \
	amiga_addtof.o \
	amiga_argarraydone.o \
	amiga_argarrayinit.o \
	amiga_argint.o \
	amiga_argstring.o \
	amiga_beginio.o \
	amiga_callhooka.o \
	amiga_callhook.o \
	amiga_coercemethoda.o \
	amiga_coercemethod.o \
	amiga_createextio.o \
	amiga_createport.o \
	amiga_createstdio.o \
	amiga_createtask.o \
	amiga_deleteextio.o \
	amiga_deleteport.o \
	amiga_deletestdio.o \
	amiga_deletetask.o \
	amiga_domethoda.o \
	amiga_domethod.o \
	amiga_dosupermethoda.o \
	amiga_dosupermethod.o \
	amiga_dotimer.o \
	amiga_fastrand.o \
	amiga_ffp.o \
	amiga_freeievents.o \
	amiga_hotkey.o \
	amiga_invertstring.o \
	amiga_liballocpooled.o \
	amiga_libcreatepool.o \
	amiga_libdeletepool.o \
	amiga_libfreepooled.o \
	amiga_newlist.o \
	amiga_rangerand.o \
	amiga_remtof.o \
	amiga_rexxvars.o \
	amiga_setsuperattrs.o \
	amiga_timedelay.o \
	amiga_waitbeam.o \
	string_memchr.o

AMIGA_LIB_ASM = \
	amiga_addtof_asm.o \
	amiga_ffp_asm.o \
	amiga_hookentry.o

MEMDEBUG_LIB = \
	stdio_vasprintf_debug.o \
	stdio_vasprintf_debug.o \
	stdio_vasprintf_hook_entry_debug.o \
	stdlib_alloca_debug.o \
	stdlib_calloc_debug.o \
	stdlib_free_debug.o \
	stdlib_malloc_debug.o \
	string_strdup_debug.o \
	unistd_getcwd_debug.o

ifdef WINDOWS
	DELETE = @del /q
	AR = @ar crs
else
	DELETE = @$(RM) -rf
	AR = @$(AR) crs
endif

subdirs := $(wildcard */)
VPATH = $(subdirs) 
LIBC_OBJS	= obj/libc_objs
LIBUNIX_OBJS	= obj/libunix_objs
LIBM_OBJS	= obj/libm_objs
LIBM881_OBJS	= obj/libm881_objs
LIBNET_OBJS	= obj/libnet_objs
LIBDEBUG_OBJS	= obj/libdebug_objs
LIBAMIGA_OBJS	= obj/libamiga_objs

c_clib_objects := $(addprefix $(LIBC_OBJS)/,$(C_LIB))
asm_clib_objects := $(addprefix $(LIBC_OBJS)/,$(C_LIB_ASM))
c_unixlib_objects := $(addprefix $(LIBUNIX_OBJS)/,$(UNIX_LIB))
c_mathlibieee_objects := $(addprefix $(LIBM_OBJS)/,$(MATH_LIB_IEEE))
c_mathlib881_objects := $(addprefix $(LIBM881_OBJS)/,$(MATH_LIB_881))
c_netlib_objects := $(addprefix $(LIBNET_OBJS)/,$(NET_LIB))
c_debuglib_objects := $(addprefix $(LIBDEBUG_OBJS)/,$(DEBUG_LIB))
c_amigalib_objects := $(addprefix $(LIBAMIGA_OBJS)/,$(AMIGA_LIB))
asm_amigalib_objects := $(addprefix $(LIBAMIGA_OBJS)/,$(AMIGA_LIB_ASM))
c_test_prg_objects := $(addprefix obj/,$(TEST_PRG))

objects := $(c_clib_objects) $(asm_clib_objects) $(c_unixlib_objects) $(c_mathlibieee_objects) $(c_mathlib881_objects) $(c_netlib_objects) $(c_debuglib_objects) $(c_amigalib_objects) $(asm_amigalib_objects) $(c_test_prg_objects)

# This is the place to declare the static libraries
static_libs := -lc -lm -ldebug -lnet -lunix -lc -lamiga # to use Amiga paths
#static_libs := -lm -ldebug -lnet -lunix -lc -lamiga # to use UNIX paths

# https://stackoverflow.com/questions/4036191/sources-from-subdirectories-in-makefile/4038459
# http://www.microhowto.info/howto/automatically_generate_makefile_dependencies.html

program = out/a
OUT = $(program)
CC = m68k-amiga-elf-gcc
VASM = vasmm68k_mot

ifdef WINDOWS
	SDKDIR = $(abspath $(dir $(shell where $(CC)))..\m68k-amiga-elf\sys-include)
	MUISDKDIR = $(abspath $(dir $(shell where $(CC)))..\m68k-amiga-elf\sys-include\SDK\MUI\C\include)
else
	SDKDIR = $(abspath $(dir $(shell which $(CC)))../m68k-amiga-elf/sys-include)
	MUISDKDIR = $(abspath $(dir $(shell which $(CC)))../m68k-amiga-elf/sys-include/SDK/MUI/C/include)
endif

# The directory where the static libraries are located
LIBSDIR = $(abspath libs)

# clib2 include directory
CLIB2INCLUDEDIR = $(abspath library)

BASECCFLAGS = -MP -MMD -Ofast -nostdlib -Wextra -Wno-unused-function -Wno-volatile-register-var -fomit-frame-pointer -fno-tree-loop-distribution -flto -fno-exceptions \
-I${MUISDKDIR} -I$(LIBSDIR)/alib -I$(CLIB2INCLUDEDIR) -I$(CLIB2INCLUDEDIR)/include -D__stdargs= \
-fno-builtin -DNDEBUG -D__THREAD_SAFE
CCFLAGS = $(BASECCFLAGS) -m68000
CCFLAGS881 = $(BASECCFLAGS) -m68020 -m68881
LDFLAGS = -Wl,--emit-relocs,-Ttext=0,-Map=$(OUT).map -L${LIBSDIR}
VASMFLAGS = -m68000 -Felf -opt-fconst -nowarn=62 -dwarf=3 -quiet -x -I. -I$(SDKDIR)

all: $(OUT).exe

libs/libc.a : libs $(foreach file,$(c_clib_objects) $(asm_clib_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_clib_objects) $(asm_clib_objects),$(file))

libs/libunix.a : libs $(foreach file,$(c_unixlib_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_unixlib_objects),$(file))

libs/libm.a : libs $(foreach file,$(c_mathlibieee_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_mathlibieee_objects),$(file))

libs/libm881.a : libs $(foreach file,$(c_mathlib881_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_mathlib881_objects),$(file))
	
libs/libnet.a : libs $(foreach file,$(c_netlib_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_netlib_objects),$(file))

libs/libdebug.a : libs $(foreach file,$(c_debuglib_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_debuglib_objects),$(file))

libs/libamiga.a : libs $(foreach file,$(c_amigalib_objects) $(asm_amigalib_objects),$(file))
	$(DELETE) $(subst $@,/,\)
	@$(AR) $@ $(foreach file,$(c_amigalib_objects) $(asm_amigalib_objects),$(file))

$(OUT).exe: $(OUT).elf
	$(info Elf2Hunk $(program).exe)
	@elf2hunk $(OUT).elf $(OUT).exe -s

$(OUT).elf: libs/libc.a libs/libunix.a libs/libm.a libs/libm881.a libs/libnet.a libs/libdebug.a libs/libamiga.a $(TEST_PRG)
	$(info Linking $(program).elf)
	@$(CC) $(CCFLAGS) $(LDFLAGS) $(foreach file,$(c_test_prg_objects),$(file)) $(static_libs) -o $@
	@m68k-amiga-elf-objdump --disassemble --no-show-raw-ins --visualize-jumps -S $@ >$(OUT).s

clean:
	$(info Cleaning...)
ifdef WINDOWS
	@del /q obj\* obj\libamiga_objs\* obj\libc_objs\* obj\libdebug_objs\* obj\libdebug_objs\* obj\libm881_objs\* obj\libnet_objs\* obj\libunix_objs\* out\* libs\*
else
	@$(RM) obj/* out/*
endif

-include $(objects:.o=.d)

$(c_clib_objects) :$(LIBC_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -DIEEE_FLOATING_POINT_SUPPORT -c -o $@ $(CURDIR)/$<

$(c_unixlib_objects) : $(LIBUNIX_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -DUNIX_PATH_SEMANTICS -c -o $@ $(CURDIR)/$<

$(c_mathlib881_objects) : $(LIBM881_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS881) -DM68881_FLOATING_POINT_SUPPORT -c -o $@ $(CURDIR)/$<

$(c_mathlibieee_objects) : $(LIBM_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -DIEEE_FLOATING_POINT_SUPPORT -c -o $@ $(CURDIR)/$<

$(c_netlib_objects) : $(LIBNET_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -DSOCKET_SUPPORT -DUSERGROUP_SUPPORT -c -o $@ $(CURDIR)/$<

$(c_debuglib_objects) : $(LIBDEBUG_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -c -o $@ $(CURDIR)/$<

$(c_amigalib_objects) : $(LIBAMIGA_OBJS)/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -c -o $@ $(CURDIR)/$<

$(asm_clib_objects): $(LIBC_OBJS)/%.o : %.asm
	$(info Assembling $<)
	@$(VASM) $(VASMFLAGS) -o $@ $(CURDIR)/$<

$(asm_amigalib_objects): $(LIBAMIGA_OBJS)/%.o : %.asm
	$(info Assembling $<)
	@$(VASM) $(VASMFLAGS) -o $@ $(CURDIR)/$<

$(c_test_prg_objects) : obj/%.o : %.c
	$(info Compiling $<)
	@$(CC) $(CCFLAGS) -DIEEE_FLOATING_POINT_SUPPORT -c -o $@ $(CURDIR)/$<

CONSTRUCTOR_FILES = \
	amiga_rexxvars.c \
	dirent_closedir.c \
	locale_init_exit.c \
	math_init_exit.c \
	socket_init_exit.c \
	stdio_file_init.c \
	stdio_init_exit.c \
	stdlib_alloca.c \
	stdlib_arg.c \
	stdlib_malloc.c \
	stdlib_program_name.c \
	stdlib_setenv.c \
	time_clock.c \
	unistd_chdir_exit.c \
	unistd_init_exit.c \
	unistd_timer.c \
	unistd_wildcard_expand.c \
	usergroup_init_exit.c

touch_constructor_files:
	touch $(CONSTRUCTOR_FILES)