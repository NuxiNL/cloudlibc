// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(sysconf, posix) {
  errno = 0;

  // Options defined by POSIX.
  ASSERT_EQ(_POSIX2_C_BIND, sysconf(_SC_2_C_BIND));
  ASSERT_EQ(_POSIX2_C_DEV, sysconf(_SC_2_C_DEV));
  ASSERT_EQ(_POSIX2_CHAR_TERM, sysconf(_SC_2_CHAR_TERM));
  ASSERT_EQ(_POSIX2_FORT_DEV, sysconf(_SC_2_FORT_DEV));
  ASSERT_EQ(_POSIX2_FORT_RUN, sysconf(_SC_2_FORT_RUN));
  ASSERT_EQ(_POSIX2_LOCALEDEF, sysconf(_SC_2_LOCALEDEF));
#ifdef _POSIX2_PBS
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_2_PBS));
#ifdef _POSIX2_PBS_ACCOUNTING
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_2_PBS_ACCOUNTING));
#ifdef _POSIX2_PBS_CHECKPOINT
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_2_PBS_CHECKPOINT));
#ifdef _POSIX2_PBS_LOCATE
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_2_PBS_LOCATE));
#ifdef _POSIX2_PBS_MESSAGE
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_2_PBS_MESSAGE));
#ifdef _POSIX2_PBS_TRACK
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_2_PBS_TRACK));
  ASSERT_EQ(_POSIX2_SW_DEV, sysconf(_SC_2_SW_DEV));
  ASSERT_EQ(_POSIX2_UPE, sysconf(_SC_2_UPE));
#ifdef _SC_2_VERSION
#error "Test missing"
#endif
  ASSERT_EQ(_POSIX_ADVISORY_INFO, sysconf(_SC_ADVISORY_INFO));
#ifdef AIO_LISTIO_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_AIO_LISTIO_MAX));
#ifdef AIO_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_AIO_MAX));
#ifdef AIO_PRIO_DELTA_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_AIO_PRIO_DELTA_MAX));
#ifdef ARG_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_ARG_MAX));
  ASSERT_EQ(_POSIX_ASYNCHRONOUS_IO, sysconf(_SC_ASYNCHRONOUS_IO));
#ifdef ATEXIT_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_ATEXIT_MAX));
  ASSERT_EQ(_POSIX_BARRIERS, sysconf(_SC_BARRIERS));
#ifdef BC_BASE_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_BC_BASE_MAX));
#ifdef BC_DIM_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_BC_DIM_MAX));
#ifdef BC_SCALE_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_BC_SCALE_MAX));
#ifdef BC_STRING_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_BC_STRING_MAX));
#ifdef CHILD_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_CHILD_MAX));
#ifdef CLK_TCK
#error "Test missing"
#endif
  ASSERT_EQ(CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));
  ASSERT_EQ(_POSIX_CLOCK_SELECTION, sysconf(_SC_CLOCK_SELECTION));
#ifdef COLL_WEIGHTS_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_COLL_WEIGHTS_MAX));
  ASSERT_EQ(_POSIX_CPUTIME, sysconf(_SC_CPUTIME));
#ifdef DELAYTIMER_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_DELAYTIMER_MAX));
#ifdef EXPR_NEST_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_EXPR_NEST_MAX));
  ASSERT_EQ(_POSIX_FSYNC, sysconf(_SC_FSYNC));
  ASSERT_EQ(-1, sysconf(_SC_GETGR_R_SIZE_MAX));
  ASSERT_EQ(-1, sysconf(_SC_GETPW_R_SIZE_MAX));
#ifdef HOST_NAME_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_HOST_NAME_MAX));
#ifdef IOV_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_IOV_MAX));
  ASSERT_EQ(_POSIX_IPV6, sysconf(_SC_IPV6));
  ASSERT_EQ(_POSIX_JOB_CONTROL, sysconf(_SC_JOB_CONTROL));
#ifdef LINE_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_LINE_MAX));
#ifdef LOGIN_NAME_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_LOGIN_NAME_MAX));
  ASSERT_EQ(_POSIX_MAPPED_FILES, sysconf(_SC_MAPPED_FILES));
  ASSERT_EQ(_POSIX_MEMLOCK, sysconf(_SC_MEMLOCK));
  ASSERT_EQ(_POSIX_MEMLOCK_RANGE, sysconf(_SC_MEMLOCK_RANGE));
  ASSERT_EQ(_POSIX_MEMORY_PROTECTION, sysconf(_SC_MEMORY_PROTECTION));
  ASSERT_EQ(_POSIX_MESSAGE_PASSING, sysconf(_SC_MESSAGE_PASSING));
  ASSERT_EQ(_POSIX_MONOTONIC_CLOCK, sysconf(_SC_MONOTONIC_CLOCK));
#ifdef MQ_OPEN_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_MQ_OPEN_MAX));
#ifdef MQ_PRIO_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_MQ_PRIO_MAX));
#ifdef NGROUPS_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_NGROUPS_MAX));
#ifdef OPEN_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_OPEN_MAX));
  ASSERT_LE(1, sysconf(_SC_PAGE_SIZE));
  ASSERT_EQ(sysconf(_SC_PAGE_SIZE), sysconf(_SC_PAGESIZE));
  ASSERT_EQ(_POSIX_PRIORITIZED_IO, sysconf(_SC_PRIORITIZED_IO));
  ASSERT_EQ(_POSIX_PRIORITY_SCHEDULING, sysconf(_SC_PRIORITY_SCHEDULING));
  ASSERT_EQ(_POSIX_RAW_SOCKETS, sysconf(_SC_RAW_SOCKETS));
  ASSERT_EQ(RE_DUP_MAX, sysconf(_SC_RE_DUP_MAX));
  ASSERT_EQ(_POSIX_READER_WRITER_LOCKS, sysconf(_SC_READER_WRITER_LOCKS));
  ASSERT_EQ(_POSIX_REALTIME_SIGNALS, sysconf(_SC_REALTIME_SIGNALS));
  ASSERT_EQ(_POSIX_REGEXP, sysconf(_SC_REGEXP));
#ifdef RTSIG_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_RTSIG_MAX));
  ASSERT_EQ(_POSIX_SAVED_IDS, sysconf(_SC_SAVED_IDS));
#ifdef SEM_NSEMS_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_SEM_NSEMS_MAX));
  ASSERT_EQ(SEM_VALUE_MAX, sysconf(_SC_SEM_VALUE_MAX));
  ASSERT_EQ(_POSIX_SEMAPHORES, sysconf(_SC_SEMAPHORES));
  ASSERT_EQ(_POSIX_SHARED_MEMORY_OBJECTS, sysconf(_SC_SHARED_MEMORY_OBJECTS));
  ASSERT_EQ(_POSIX_SHELL, sysconf(_SC_SHELL));
#ifdef SIGQUEUE_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_SIGQUEUE_MAX));
  ASSERT_EQ(_POSIX_SPAWN, sysconf(_SC_SPAWN));
  ASSERT_EQ(_POSIX_SPIN_LOCKS, sysconf(_SC_SPIN_LOCKS));
  ASSERT_EQ(_POSIX_SPORADIC_SERVER, sysconf(_SC_SPORADIC_SERVER));
#ifdef SS_REPL_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_SS_REPL_MAX));
#ifdef STREAM_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_STREAM_MAX));
#ifdef SYMLOOP_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_SYMLOOP_MAX));
  ASSERT_EQ(_POSIX_SYNCHRONIZED_IO, sysconf(_SC_SYNCHRONIZED_IO));
  ASSERT_EQ(_POSIX_THREAD_ATTR_STACKADDR, sysconf(_SC_THREAD_ATTR_STACKADDR));
  ASSERT_EQ(_POSIX_THREAD_ATTR_STACKSIZE, sysconf(_SC_THREAD_ATTR_STACKSIZE));
  ASSERT_EQ(_POSIX_THREAD_CPUTIME, sysconf(_SC_THREAD_CPUTIME));
  ASSERT_EQ(PTHREAD_DESTRUCTOR_ITERATIONS,
            sysconf(_SC_THREAD_DESTRUCTOR_ITERATIONS));
#ifdef PTHREAD_KEYS_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_THREAD_KEYS_MAX));
  ASSERT_EQ(_POSIX_THREAD_PRIO_INHERIT, sysconf(_SC_THREAD_PRIO_INHERIT));
  ASSERT_EQ(_POSIX_THREAD_PRIO_PROTECT, sysconf(_SC_THREAD_PRIO_PROTECT));
  ASSERT_EQ(_POSIX_THREAD_PRIORITY_SCHEDULING,
            sysconf(_SC_THREAD_PRIORITY_SCHEDULING));
  ASSERT_EQ(_POSIX_THREAD_PROCESS_SHARED, sysconf(_SC_THREAD_PROCESS_SHARED));
  ASSERT_EQ(_POSIX_THREAD_ROBUST_PRIO_INHERIT,
            sysconf(_SC_THREAD_ROBUST_PRIO_INHERIT));
  ASSERT_EQ(_POSIX_THREAD_ROBUST_PRIO_PROTECT,
            sysconf(_SC_THREAD_ROBUST_PRIO_PROTECT));
  ASSERT_EQ(_POSIX_THREAD_SAFE_FUNCTIONS, sysconf(_SC_THREAD_SAFE_FUNCTIONS));
  ASSERT_EQ(_POSIX_THREAD_SPORADIC_SERVER, sysconf(_SC_THREAD_SPORADIC_SERVER));
  ASSERT_EQ(PTHREAD_STACK_MIN, sysconf(_SC_THREAD_STACK_MIN));
#ifdef PTHREAD_THREADS_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_THREAD_THREADS_MAX));
  ASSERT_EQ(_POSIX_THREADS, sysconf(_SC_THREADS));
  ASSERT_EQ(_POSIX_TIMEOUTS, sysconf(_SC_TIMEOUTS));
#ifdef TIMER_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TIMER_MAX));
  ASSERT_EQ(_POSIX_TIMERS, sysconf(_SC_TIMERS));
#ifdef _POSIX_TRACE
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE));
#ifdef _POSIX_TRACE_EVENT_FILTER
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_EVENT_FILTER));
#ifdef _POSIX_TRACE_EVENT_NAME_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_EVENT_NAME_MAX));
#ifdef _POSIX_TRACE_INHERIT
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_INHERIT));
#ifdef _POSIX_TRACE_LOG
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_LOG));
#ifdef _POSIX_TRACE_NAME_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_NAME_MAX));
#ifdef _POSIX_TRACE_SYS_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_SYS_MAX));
#ifdef _POSIX_TRACE_USER_EVENT_MAX
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TRACE_USER_EVENT_MAX));
#ifdef TTY_NAME_MAX
#define "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_TTY_NAME_MAX));
  ASSERT_EQ(_POSIX_TYPED_MEMORY_OBJECTS, sysconf(_SC_TYPED_MEMORY_OBJECTS));
#ifdef TZNAME_MAX
#define "Test missing"
#endif
  ASSERT_EQ(_POSIX_V7_ILP32_OFF32, sysconf(_SC_V7_ILP32_OFF32));
  ASSERT_EQ(_POSIX_V7_ILP32_OFFBIG, sysconf(_SC_V7_ILP32_OFFBIG));
  ASSERT_EQ(_POSIX_V7_LP64_OFF64, sysconf(_SC_V7_LP64_OFF64));
  ASSERT_EQ(_POSIX_V7_LPBIG_OFFBIG, sysconf(_SC_V7_LPBIG_OFFBIG));
#ifdef _SC_VERSION
#error "Test missing"
#endif
  ASSERT_EQ(_XOPEN_CRYPT, sysconf(_SC_XOPEN_CRYPT));
  ASSERT_EQ(_XOPEN_ENH_I18N, sysconf(_SC_XOPEN_ENH_I18N));
  ASSERT_EQ(_XOPEN_REALTIME, sysconf(_SC_XOPEN_REALTIME));
  ASSERT_EQ(_XOPEN_REALTIME_THREADS, sysconf(_SC_XOPEN_REALTIME_THREADS));
  ASSERT_EQ(_XOPEN_SHM, sysconf(_SC_XOPEN_SHM));
#ifdef _XOPEN_STREAMS
#error "Test missing"
#endif
  ASSERT_EQ(-1, sysconf(_SC_XOPEN_STREAMS));
  ASSERT_EQ(_XOPEN_UNIX, sysconf(_SC_XOPEN_UNIX));
  ASSERT_EQ(_XOPEN_UUCP, sysconf(_SC_XOPEN_UUCP));
#ifdef _SC_XOPEN_VERSION
#error "Test missing"
#endif

  // None of these options should set errno.
  ASSERT_EQ(0, errno);
}

TEST(sysconf, extensions) {
  ASSERT_LE(1, sysconf(_SC_NPROCESSORS_CONF));
  ASSERT_LE(1, sysconf(_SC_NPROCESSORS_ONLN));
}

TEST(sysconf, bad) {
  ASSERT_EQ(-1, sysconf(0xdeadc0de));
  ASSERT_EQ(EINVAL, errno);
}
