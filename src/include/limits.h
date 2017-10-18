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

// <limits.h> - implementation-defined constants
//
// Features missing:
// - BC_BASE_MAX, BC_DIM_MAX, BC_SCALE_MAX, BC_STRING_MAX, EXPR_NEST_MAX and
//   LINE_MAX:
//   Command line utilities cannot be invoked.
// - COLL_WEIGHTS_MAX:
//   Collation weights cannot be adjusted at run-time.
// - NL_MSGMAX, NL_SETMAX:
//   Message catalogs are not supported.
// - NZERO:
//   Scheduler interaction is not supported.

#ifndef _LIMITS_H_
#define _LIMITS_H_

#include <_/limits.h>

// Runtime invariant values.

// AIO_LISTIO_MAX: Asynchronous I/O not available.
// AIO_MAX: Asynchronous I/O not available.
// AIO_PRIO_DELTA_MAX: Asynchronous I/O not available.
// ARG_MAX: Indeterminate.
// ATEXIT_MAX: Indeterminate.
// CHILD_MAX: Indeterminate.
// DELAYTIMER_MAX: Timer interface not available.
// HOST_NAME_MAX: System hostname not available.
// IOV_MAX: Indeterminate.
// LOGIN_NAME_MAX: User credentials are not available.
// MQ_OPEN_MAX: Indeterminate.
// MQ_PRIO_MAX: Indeterminate.
// OPEN_MAX: Indeterminate.
// PAGESIZE: Indeterminate.
// PAGE_SIZE: Indeterminate.
#define PTHREAD_DESTRUCTOR_ITERATIONS 64
// PTHREAD_KEYS_MAX: Indeterminate.
#define PTHREAD_STACK_MIN 1024
// PTHREAD_THREADS_MAX: Indeterminate.
#define RE_DUP_MAX 255
// RTSIG_MAX: Realtime signals are not supported.
// SEM_NSEMS_MAX: Indeterminate.
#define SEM_VALUE_MAX _UINT32_MAX
// SIGQUEUE_MAX: Signal handling is not available.
// STREAM_MAX: Streams are not supported.
// SYMLOOP_MAX: Indeterminate.
// TIMER_MAX: Timer interface not available.
// TTY_NAME_MAX: Terminal interaction is not supported.
// TZNAME_MAX: Indeterminate.

// Pathname variable values.

// FILESIZEBITS: Indeterminate.
// LINK_MAX: Indeterminate.
// MAX_CANON: Terminal interaction is not supported.
// MAX_INPUT: Terminal interaction is not supported.
// NAME_MAX: Indeterminate.
// PATH_MAX: Indeterminate.
// PIPE_BUF: Indeterminate.
// POSIX_ALLOC_SIZE_MIN: Indeterminate.
// POSIX_REC_INCR_XFER_SIZE: Indeterminate.
// POSIX_REC_MAX_XFER_SIZE: Indeterminate.
// POSIX_REC_MIN_XFER_SIZE: Indeterminate.
// POSIX_REC_XFER_ALIGN: Indeterminate.
// SYMLINK_MAX: Indeterminate.

// Runtime increasable values.

#define CHARCLASS_NAME_MAX 32

// Maximum values.

#define _POSIX_CLOCKRES_MIN 20000000

// Minimum values.

#define _POSIX_AIO_LISTIO_MAX 2
#define _POSIX_AIO_MAX 1
#define _POSIX_ARG_MAX 4096
#define _POSIX_CHILD_MAX 25
#define _POSIX_DELAYTIMER_MAX 32
#define _POSIX_HOST_NAME_MAX 255
#define _POSIX_LINK_MAX 8
#define _POSIX_LOGIN_NAME_MAX 9
#define _POSIX_MAX_CANON 255
#define _POSIX_MAX_INPUT 255
#define _POSIX_MQ_OPEN_MAX 8
#define _POSIX_MQ_PRIO_MAX 32
#define _POSIX_NAME_MAX 14
#define _POSIX_NGROUPS_MAX 8
#define _POSIX_OPEN_MAX 20
#define _POSIX_PATH_MAX 256
#define _POSIX_PIPE_BUF 512
#define _POSIX_RE_DUP_MAX 255
#define _POSIX_RTSIG_MAX 8
#define _POSIX_SEM_NSEMS_MAX 256
#define _POSIX_SEM_VALUE_MAX 32767
#define _POSIX_SIGQUEUE_MAX 32
#define _POSIX_SSIZE_MAX 32767
#define _POSIX_SS_REPL_MAX 4
#define _POSIX_STREAM_MAX 8
#define _POSIX_SYMLINK_MAX 255
#define _POSIX_SYMLOOP_MAX 8
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4
#define _POSIX_THREAD_KEYS_MAX 128
#define _POSIX_THREAD_THREADS_MAX 64
#define _POSIX_TIMER_MAX 32
#define _POSIX_TTY_NAME_MAX 9
#define _POSIX_TZNAME_MAX 6
#define _POSIX2_BC_BASE_MAX 99
#define _POSIX2_BC_DIM_MAX 2048
#define _POSIX2_BC_SCALE_MAX 99
#define _POSIX2_BC_STRING_MAX 1000
#define _POSIX2_CHARCLASS_NAME_MAX 14
#define _POSIX2_COLL_WEIGHTS_MAX 2
#define _POSIX2_EXPR_NEST_MAX 32
#define _POSIX2_LINE_MAX 2048
#define _POSIX2_RE_DUP_MAX 255
#define _XOPEN_IOV_MAX 16
#define _XOPEN_NAME_MAX 255
#define _XOPEN_PATH_MAX 1024

// Numerical limits.

#define CHAR_BIT _CHAR_BIT
#define CHAR_MAX _CHAR_MAX
#define CHAR_MIN _CHAR_MIN
#define SCHAR_MIN _SCHAR_MIN
#define SCHAR_MAX _SCHAR_MAX
#define UCHAR_MAX _UCHAR_MAX
#define SHRT_MIN _SHRT_MIN
#define SHRT_MAX _SHRT_MAX
#define USHRT_MAX _USHRT_MAX
#define WORD_BIT _INT_BIT
#define INT_MIN _INT_MIN
#define INT_MAX _INT_MAX
#define UINT_MAX _UINT_MAX
#define LONG_BIT _LONG_BIT
#define LONG_MIN _LONG_MIN
#define LONG_MAX _LONG_MAX
#define ULONG_MAX _ULONG_MAX
#define LLONG_MIN _LLONG_MIN
#define LLONG_MAX _LLONG_MAX
#define ULLONG_MAX _ULLONG_MAX

#define SSIZE_MAX _SSIZE_MAX

#define MB_LEN_MAX 4

// Other invariant values.

#define NL_ARGMAX 128  // Not actually limited.
#define NL_LANGMAX 128
#define NL_TEXTMAX 2048

#endif
