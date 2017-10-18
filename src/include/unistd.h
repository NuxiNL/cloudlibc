// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <unistd.h> - standard symbolic constants and types
//
// Extensions:
// - _SC_NPROCESSORS_CONF and _SC_NPROCESSORS_ONLN:
//   Present on most systems and used by many pieces of software.
// - struct crypt_data and crypt_r():
//   Thread-safe replacement for crypt(). Present on Linux as well.
// - pipe():
//   As this implementation does not distinguish between pipes and
//   socket pairs, this function creates a bidirectional pipe that
//   supports socket operations.
// - useconds_t and usleep():
//   Still used actively, as it was part of POSIX up to issue 6.
//
// Features missing:
// - _POSIX_VERSION, _POSIX2_VERSION, _XOPEN_VERSION, _SC_VERSION,
//   _SC_2_VERSION and _SC_XOPEN_VERSION:
//   System does not conform to POSIX.
// - faccessat():
//   Validates file descriptor rights; not filesystem access controls.
// - fchown() and fchownat():
//   Filesystem access control management not available.
// - _CS_* and confstr():
//   Not applicable, as POSIX command line tools are not available.
// - F_LOCK, F_TEST, F_TLOCK, F_ULOCK and lockf():
//   File locking not available.
// - STDERR_FILENO, STDIN_FILENO and STDOUT_FILENO:
//   Standard descriptors are not available.
// - _POSIX_VDISABLE, _PC_MAX_CANON, _PC_MAX_INPUT, _POSIX_VDISABLE,
//   isatty(), tcgetpgrp(), tcsetpgrp(), ttyname() and ttyname_r():
//   Terminal management is not available.
// - uid_t, gid_t, getegid(), geteuid(), getgid(), getgroups(),
//   getlogin(), getlogin_r(), getuid(), setegid(), seteuid(), setgid(),
//   setregid(), setreuid() and setuid():
//   POSIX credential management is not available.
// - pid_t, fork(), getpgid(), getpgrp(), getpid(), getppid(), getsid(),
//   setpgid() and setsid():
//   Requires global process namespace. Use pdfork() instead.
// - access(), chown(), execl(), execle(), execlp(), execv(), execve(),
//   execvp(), lchown(), link(), pathconf(), readlink(),
//   rmdir(), symlink(), truncate() and unlink():
//   Requires global filesystem namespace.
// - alarm() and pause():
//   Signal handling not available.
// - chdir(), fchdir(), getcwd():
//   Per-process working directory is not available. Use *at() instead.
// - crypt():
//   Not thread-safe. Use crypt_r() instead.
// - encrypt():
//   Encryption schemes not available.
// - fexecve():
//   Use <program.h>'s program_exec() instead.
// - gethostid() and gethostname():
//   System-specific information not available.
// - getopt(), optarg, opterr, optind and optopt:
//   Not thread-safe.
// - nice():
//   Scheduler interaction not available.
// - sync():
//   Use fsync() and fdatasync() instead.

#ifndef _UNISTD_H_
#define _UNISTD_H_

#include <_/types.h>

// Compile-time constants for POSIX options and option groups.
#define _POSIX2_CHAR_TERM (-1)
#define _POSIX2_C_BIND 200809L
#define _POSIX2_C_DEV (-1)
#define _POSIX2_FORT_DEV (-1)
#define _POSIX2_FORT_RUN (-1)
#define _POSIX2_LOCALEDEF (-1)
#define _POSIX2_SW_DEV (-1)
#define _POSIX2_UPE (-1)
#define _POSIX_ADVISORY_INFO 200809L
#define _POSIX_ASYNCHRONOUS_IO (-1)
#define _POSIX_BARRIERS 200809L
#define _POSIX_CHOWN_RESTRICTED (-1)
#define _POSIX_CLOCK_SELECTION 200809L
#define _POSIX_CPUTIME 200809L
#define _POSIX_FSYNC 200809L
#define _POSIX_IPV6 200809L
#define _POSIX_JOB_CONTROL (-1)
#define _POSIX_MAPPED_FILES 200809L
#define _POSIX_MEMLOCK (-1)
#define _POSIX_MEMLOCK_RANGE 200809L
#define _POSIX_MEMORY_PROTECTION 200809L
#define _POSIX_MESSAGE_PASSING 200809L
#define _POSIX_MONOTONIC_CLOCK 200809L
#define _POSIX_NO_TRUNC 200809L
#define _POSIX_PRIORITIZED_IO (-1)
#define _POSIX_PRIORITY_SCHEDULING (-1)
#define _POSIX_RAW_SOCKETS 200809L
#define _POSIX_READER_WRITER_LOCKS 200809L
#define _POSIX_REALTIME_SIGNALS (-1)
#define _POSIX_REGEXP 200809L
#define _POSIX_SAVED_IDS (-1)
#define _POSIX_SEMAPHORES 200809L
#define _POSIX_SHARED_MEMORY_OBJECTS 200809L
#define _POSIX_SHELL (-1)
#define _POSIX_SPAWN (-1)
#define _POSIX_SPIN_LOCKS 200809L
#define _POSIX_SPORADIC_SERVER (-1)
#define _POSIX_SYNCHRONIZED_IO 200809L
#define _POSIX_THREADS 200809L
#define _POSIX_THREAD_ATTR_STACKADDR (-1)
#define _POSIX_THREAD_ATTR_STACKSIZE 200809L
#define _POSIX_THREAD_CPUTIME 200809L
#define _POSIX_THREAD_PRIORITY_SCHEDULING (-1)
#define _POSIX_THREAD_PRIO_INHERIT 200809L
#define _POSIX_THREAD_PRIO_PROTECT (-1)
#define _POSIX_THREAD_PROCESS_SHARED 200809L
#define _POSIX_THREAD_ROBUST_PRIO_INHERIT (-1)
#define _POSIX_THREAD_ROBUST_PRIO_PROTECT (-1)
#define _POSIX_THREAD_SAFE_FUNCTIONS 200809L
#define _POSIX_THREAD_SPORADIC_SERVER (-1)
#define _POSIX_TIMEOUTS 200809L
#define _POSIX_TIMERS (-1)
#define _POSIX_TYPED_MEMORY_OBJECTS (-1)
#define _POSIX_V7_ILP32_OFF32 (-1)
#define _POSIX_V7_ILP32_OFFBIG (-1)
#define _POSIX_V7_LP64_OFF64 (-1)
#define _POSIX_V7_LPBIG_OFFBIG (-1)
#define _XOPEN_CRYPT (-1)
#define _XOPEN_ENH_I18N 200809L
#define _XOPEN_REALTIME (-1)
#define _XOPEN_REALTIME_THREADS (-1)
#define _XOPEN_SHM (-1)
#define _XOPEN_UNIX (-1)
#define _XOPEN_UUCP (-1)

#define _POSIX_PRIO_IO (-1)

#define NULL _NULL

#define F_OK 0    // Test for existence of file.
#define R_OK 0x1  // Test for read permission.
#define W_OK 0x2  // Test for write permission.
#define X_OK 0x4  // Test for execute (search) permission.

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 3

// fpathconf() options.
#define _PC_2_SYMLINKS 1
#define _PC_ALLOC_SIZE_MIN 2
#define _PC_ASYNC_IO 3
#define _PC_CHOWN_RESTRICTED 4
#define _PC_FILESIZEBITS 5
#define _PC_LINK_MAX 6
#define _PC_NAME_MAX 7
#define _PC_NO_TRUNC 8
#define _PC_PATH_MAX 9
#define _PC_PIPE_BUF 10
#define _PC_PRIO_IO 11
#define _PC_REC_INCR_XFER_SIZE 12
#define _PC_REC_MAX_XFER_SIZE 13
#define _PC_REC_MIN_XFER_SIZE 14
#define _PC_REC_XFER_ALIGN 15
#define _PC_SYMLINK_MAX 16
#define _PC_SYNC_IO 17
#define _PC_TIMESTAMP_RESOLUTION 18

// sysconf() options.
#define _SC_2_CHAR_TERM 1
#define _SC_2_C_BIND 2
#define _SC_2_C_DEV 3
#define _SC_2_FORT_DEV 4
#define _SC_2_FORT_RUN 5
#define _SC_2_LOCALEDEF 6
#define _SC_2_PBS 7
#define _SC_2_PBS_ACCOUNTING 8
#define _SC_2_PBS_CHECKPOINT 9
#define _SC_2_PBS_LOCATE 10
#define _SC_2_PBS_MESSAGE 11
#define _SC_2_PBS_TRACK 12
#define _SC_2_SW_DEV 13
#define _SC_2_UPE 14
#define _SC_ADVISORY_INFO 15
#define _SC_AIO_LISTIO_MAX 16
#define _SC_AIO_MAX 17
#define _SC_AIO_PRIO_DELTA_MAX 18
#define _SC_ARG_MAX 19
#define _SC_ASYNCHRONOUS_IO 20
#define _SC_ATEXIT_MAX 21
#define _SC_BARRIERS 22
#define _SC_BC_BASE_MAX 23
#define _SC_BC_DIM_MAX 24
#define _SC_BC_SCALE_MAX 25
#define _SC_BC_STRING_MAX 26
#define _SC_CHILD_MAX 27
#define _SC_CLK_TCK 28
#define _SC_CLOCK_SELECTION 29
#define _SC_COLL_WEIGHTS_MAX 30
#define _SC_CPUTIME 31
#define _SC_DELAYTIMER_MAX 32
#define _SC_EXPR_NEST_MAX 33
#define _SC_FSYNC 34
#define _SC_GETGR_R_SIZE_MAX 35
#define _SC_GETPW_R_SIZE_MAX 36
#define _SC_HOST_NAME_MAX 37
#define _SC_IOV_MAX 38
#define _SC_IPV6 39
#define _SC_JOB_CONTROL 40
#define _SC_LINE_MAX 41
#define _SC_LOGIN_NAME_MAX 42
#define _SC_MAPPED_FILES 43
#define _SC_MEMLOCK 44
#define _SC_MEMLOCK_RANGE 45
#define _SC_MEMORY_PROTECTION 46
#define _SC_MESSAGE_PASSING 47
#define _SC_MONOTONIC_CLOCK 48
#define _SC_MQ_OPEN_MAX 49
#define _SC_MQ_PRIO_MAX 50
#define _SC_NGROUPS_MAX 51
#define _SC_NPROCESSORS_ONLN 52
#define _SC_OPEN_MAX 53
#define _SC_PAGESIZE 54
#define _SC_PRIORITIZED_IO 55
#define _SC_PRIORITY_SCHEDULING 56
#define _SC_RAW_SOCKETS 57
#define _SC_READER_WRITER_LOCKS 58
#define _SC_REALTIME_SIGNALS 59
#define _SC_REGEXP 60
#define _SC_RE_DUP_MAX 61
#define _SC_RTSIG_MAX 62
#define _SC_SAVED_IDS 63
#define _SC_SEMAPHORES 64
#define _SC_SEM_NSEMS_MAX 65
#define _SC_SEM_VALUE_MAX 66
#define _SC_SHARED_MEMORY_OBJECTS 67
#define _SC_SHELL 68
#define _SC_SIGQUEUE_MAX 69
#define _SC_SPAWN 70
#define _SC_SPIN_LOCKS 71
#define _SC_SPORADIC_SERVER 72
#define _SC_SS_REPL_MAX 73
#define _SC_STREAM_MAX 74
#define _SC_SYMLOOP_MAX 75
#define _SC_SYNCHRONIZED_IO 76
#define _SC_THREADS 77
#define _SC_THREAD_ATTR_STACKADDR 78
#define _SC_THREAD_ATTR_STACKSIZE 79
#define _SC_THREAD_CPUTIME 80
#define _SC_THREAD_DESTRUCTOR_ITERATIONS 81
#define _SC_THREAD_KEYS_MAX 82
#define _SC_THREAD_PRIORITY_SCHEDULING 83
#define _SC_THREAD_PRIO_INHERIT 84
#define _SC_THREAD_PRIO_PROTECT 85
#define _SC_THREAD_PROCESS_SHARED 86
#define _SC_THREAD_ROBUST_PRIO_INHERIT 87
#define _SC_THREAD_ROBUST_PRIO_PROTECT 88
#define _SC_THREAD_SAFE_FUNCTIONS 89
#define _SC_THREAD_SPORADIC_SERVER 90
#define _SC_THREAD_STACK_MIN 91
#define _SC_THREAD_THREADS_MAX 92
#define _SC_TIMEOUTS 93
#define _SC_TIMERS 94
#define _SC_TIMER_MAX 95
#define _SC_TRACE 96
#define _SC_TRACE_EVENT_FILTER 97
#define _SC_TRACE_EVENT_NAME_MAX 98
#define _SC_TRACE_INHERIT 99
#define _SC_TRACE_LOG 100
#define _SC_TRACE_NAME_MAX 101
#define _SC_TRACE_SYS_MAX 102
#define _SC_TRACE_USER_EVENT_MAX 103
#define _SC_TTY_NAME_MAX 104
#define _SC_TYPED_MEMORY_OBJECTS 105
#define _SC_TZNAME_MAX 106
#define _SC_V7_ILP32_OFF32 107
#define _SC_V7_ILP32_OFFBIG 108
#define _SC_V7_LP64_OFF64 109
#define _SC_V7_LPBIG_OFFBIG 110
#define _SC_XOPEN_CRYPT 111
#define _SC_XOPEN_ENH_I18N 112
#define _SC_XOPEN_REALTIME 113
#define _SC_XOPEN_REALTIME_THREADS 114
#define _SC_XOPEN_SHM 115
#define _SC_XOPEN_STREAMS 116
#define _SC_XOPEN_UNIX 117
#define _SC_XOPEN_UUCP 118
#define _SC_NPROCESSORS_CONF 119

#define _SC_PAGE_SIZE _SC_PAGESIZE

#ifndef _INTPTR_T_DECLARED
typedef __intptr_t intptr_t;
#define _INTPTR_T_DECLARED
#endif
#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define _OFF_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _SSIZE_T_DECLARED
typedef __ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif
#ifndef _USECONDS_T_DECLARED
typedef __useconds_t useconds_t;
#define _USECONDS_T_DECLARED
#endif

struct crypt_data {
  int initialized;  // Whether this object has been initialized (ignored).
  char __buf[256];  // String buffer returned by crypt_r().
};

__BEGIN_DECLS
int close(int);
char *crypt_r(const char *, const char *, struct crypt_data *);
int dup(int);
int dup2(int, int);
_Noreturn void _exit(int);
int faccessat(int, const char *, int, int);
int fdatasync(int);
long fpathconf(int, int);
int fsync(int);
int ftruncate(int, off_t);
int linkat(int, const char *, int, const char *, int);
off_t lseek(int, off_t, int);
int pipe(int *);
ssize_t pread(int, void *, size_t, off_t);
ssize_t pwrite(int, const void *, size_t, off_t);
ssize_t read(int, void *, size_t);
ssize_t readlinkat(int, const char *__restrict, char *__restrict, size_t);
unsigned int sleep(unsigned int);
void swab(const void *__restrict, void *__restrict, ssize_t);
int symlinkat(const char *, int, const char *);
long sysconf(int);
int unlinkat(int, const char *, int);
int usleep(useconds_t);
ssize_t write(int, const void *, size_t);
__END_DECLS

#endif
