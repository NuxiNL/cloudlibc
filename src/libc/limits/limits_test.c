// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <limits.h>

#ifdef AIO_LISTIO_MAX
#error "Test missing"
#endif
#ifdef AIO_MAX
#error "Test missing"
#endif
#ifdef AIO_PRIO_DELTA_MAX
#error "Test missing"
#endif
#ifdef ARG_MAX
#error "Test missing"
#endif
#ifdef ATEXIT_MAX
#error "Test missing"
#endif
#ifdef CHILD_MAX
#error "Test missing"
#endif
#ifdef DELAYTIMER_MAX
#error "Test missing"
#endif
#ifdef HOST_NAME_MAX
#error "Test missing"
#endif
#ifdef IOV_MAX
#error "Test missing"
#endif
#ifdef LOGIN_NAME_MAX
#error "Test missing"
#endif
#ifdef MQ_OPEN_MAX
#error "Test missing"
#endif
#ifdef MQ_PRIO_MAX
#error "Test missing"
#endif
#ifdef OPEN_MAX
#error "Test missing"
#endif
#ifdef PAGESIZE
#error "Test missing"
#endif
#ifdef PAGE_SIZE
#error "Test missing"
#endif
static_assert(PTHREAD_DESTRUCTOR_ITERATIONS >=
                  _POSIX_THREAD_DESTRUCTOR_ITERATIONS,
              "Limit too low");
#ifdef PTHREAD_KEYS_MAX
#error "Test missing"
#endif
static_assert(PTHREAD_STACK_MIN >= 0, "Limit too low");
#ifdef PTHREAD_THREADS_MAX
#error "Test missing"
#endif
static_assert(RE_DUP_MAX >= _POSIX2_RE_DUP_MAX, "Limit too low");
#ifdef RTSIG_MAX
#error "Test missing"
#endif
#ifdef SEM_NSEMS_MAX
#error "Test missing"
#endif
static_assert(SEM_VALUE_MAX >= _POSIX_SEM_VALUE_MAX, "Limit too low");
#ifdef SIGQUEUE_MAX
#error "Test missing"
#endif
#ifdef STREAM_MAX
#error "Test missing"
#endif
#ifdef SYMLOOP_MAX
#error "Test missing"
#endif
#ifdef TIMER_MAX
#error "Test missing"
#endif
#ifdef TTY_NAME_MAX
#error "Test missing"
#endif
#ifdef TZNAME_MAX
#error "Test missing"
#endif

#ifdef FILESIZEBITS
#error "Test missing"
#endif
#ifdef LINK_MAX
#error "Test missing"
#endif
#ifdef MAX_CANON
#error "Test missing"
#endif
#ifdef MAX_INPUT
#error "Test missing"
#endif
#ifdef NAME_MAX
#error "Test missing"
#endif
#ifdef PATH_MAX
#error "Test missing"
#endif
#ifdef PIPE_BUF
#error "Test missing"
#endif
#ifdef POSIX_ALLOC_SIZE_MIN
#error "Test missing"
#endif
#ifdef POSIX_REC_INCR_XFER_SIZE
#error "Test missing"
#endif
#ifdef POSIX_REC_MAX_XFER_SIZE
#error "Test missing"
#endif
#ifdef POSIX_REC_MIN_XFER_SIZE
#error "Test missing"
#endif
#ifdef POSIX_REC_XFER_ALIGN
#error "Test missing"
#endif
#ifdef SYMLINK_MAX
#error "Test missing"
#endif

#ifdef BC_BASE_MAX
#error "Test missing"
#endif
#ifdef BC_DIM_MAX
#error "Test missing"
#endif
#ifdef BC_SCALE_MAX
#error "Test missing"
#endif
#ifdef BC_STRING_MAX
#error "Test missing"
#endif
static_assert(CHARCLASS_NAME_MAX >= _POSIX2_CHARCLASS_NAME_MAX,
              "Limit too low");
#ifdef COLL_WEIGHTS_MAX
#error "Test missing"
#endif
#ifdef EXPR_NEST_MAX
#error "Test missing"
#endif
#ifdef LINE_MAX
#error "Test missing"
#endif
#ifdef NGROUPS_MAX
#error "Test missing"
#endif

static_assert(CHAR_BIT == 8, "Value mismatch");
static_assert((char)-1 < 0 ? CHAR_MAX == SCHAR_MAX : CHAR_MAX == UCHAR_MAX,
              "Value mismatch");
static_assert((char)-1 < 0 ? CHAR_MIN == SCHAR_MIN : CHAR_MIN == 0,
              "Value mismatch");
static_assert(INT_MAX >= 2147483647, "Limit too low");
static_assert(INT_MIN <= -2147483647, "Limit too low");
static_assert(LLONG_MAX >= 9223372036854775807LL, "Limit too low");
static_assert(LLONG_MIN <= -9223372036854775807LL, "Limit too low");
static_assert(LONG_BIT >= 32, "Limit too low");
static_assert(LONG_MAX >= 2147483647L, "Limit too low");
static_assert(LONG_MIN <= -2147483647L, "Limit too low");
static_assert(MB_LEN_MAX >= 1, "Limit too low");
static_assert(SCHAR_MAX == 127, "Value mismatch");
static_assert(SCHAR_MIN == -128, "Value mismatch");
static_assert(SHRT_MAX >= 32767, "Limit too low");
static_assert(SHRT_MIN <= -32767, "Limit too low");
static_assert(SSIZE_MAX >= _POSIX_SSIZE_MAX, "Limit too low");
static_assert(UCHAR_MAX == 255U, "Value mismatch");
static_assert(UINT_MAX >= 4294967295U, "Limit too low");
static_assert(ULLONG_MAX >= 18446744073709551615ULL, "Limit too low");
static_assert(ULLONG_MAX >= 4294967295UL, "Limit too low");
static_assert(USHRT_MAX >= 65535U, "Limit too low");
static_assert(WORD_BIT >= 32, "Limit too low");

static_assert(NL_ARGMAX >= 9, "Limit too low");
static_assert(NL_LANGMAX >= 14, "Limit too low");
#ifdef NL_MSGMAX
#error "Test missing"
#endif
#ifdef NL_SETMAX
#error "Test missing"
#endif
static_assert(NL_TEXTMAX >= _POSIX2_LINE_MAX, "Limit too low");
#ifdef NZERO
#error "Test missing"
#endif
