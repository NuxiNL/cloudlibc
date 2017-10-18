// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

long sysconf(int name) {
  switch (name) {
    // Options that are always supported.
    case _SC_2_C_BIND:
    case _SC_ADVISORY_INFO:
    case _SC_BARRIERS:
    case _SC_CLOCK_SELECTION:
    case _SC_CPUTIME:
    case _SC_FSYNC:
    case _SC_IPV6:
    case _SC_MAPPED_FILES:
    case _SC_MEMLOCK_RANGE:
    case _SC_MEMORY_PROTECTION:
    case _SC_MESSAGE_PASSING:
    case _SC_MONOTONIC_CLOCK:
    case _SC_RAW_SOCKETS:
    case _SC_READER_WRITER_LOCKS:
    case _SC_REGEXP:
    case _SC_SEMAPHORES:
    case _SC_SHARED_MEMORY_OBJECTS:
    case _SC_SPIN_LOCKS:
    case _SC_SYNCHRONIZED_IO:
    case _SC_THREADS:
    case _SC_THREAD_ATTR_STACKSIZE:
    case _SC_THREAD_CPUTIME:
    case _SC_THREAD_PRIO_INHERIT:
    case _SC_THREAD_PROCESS_SHARED:
    case _SC_THREAD_SAFE_FUNCTIONS:
    case _SC_TIMEOUTS:
    case _SC_XOPEN_ENH_I18N:
      return 200809L;

    // Options that are always unsupported.
    case _SC_2_CHAR_TERM:
    case _SC_2_C_DEV:
    case _SC_2_FORT_DEV:
    case _SC_2_FORT_RUN:
    case _SC_2_LOCALEDEF:
    case _SC_2_PBS:
    case _SC_2_PBS_ACCOUNTING:
    case _SC_2_PBS_CHECKPOINT:
    case _SC_2_PBS_LOCATE:
    case _SC_2_PBS_MESSAGE:
    case _SC_2_PBS_TRACK:
    case _SC_2_SW_DEV:
    case _SC_2_UPE:
    case _SC_ASYNCHRONOUS_IO:
    case _SC_JOB_CONTROL:
    case _SC_MEMLOCK:
    case _SC_PRIORITIZED_IO:
    case _SC_PRIORITY_SCHEDULING:
    case _SC_REALTIME_SIGNALS:
    case _SC_SAVED_IDS:
    case _SC_SHELL:
    case _SC_SPAWN:
    case _SC_SPORADIC_SERVER:
    case _SC_THREAD_ATTR_STACKADDR:
    case _SC_THREAD_PRIORITY_SCHEDULING:
    case _SC_THREAD_PRIO_PROTECT:
    case _SC_THREAD_ROBUST_PRIO_INHERIT:
    case _SC_THREAD_ROBUST_PRIO_PROTECT:
    case _SC_THREAD_SPORADIC_SERVER:
    case _SC_TIMERS:
    case _SC_TRACE:
    case _SC_TRACE_EVENT_FILTER:
    case _SC_TRACE_INHERIT:
    case _SC_TRACE_LOG:
    case _SC_TYPED_MEMORY_OBJECTS:
    case _SC_V7_ILP32_OFF32:
    case _SC_V7_ILP32_OFFBIG:
    case _SC_V7_LP64_OFF64:
    case _SC_V7_LPBIG_OFFBIG:
    case _SC_XOPEN_CRYPT:
    case _SC_XOPEN_REALTIME:
    case _SC_XOPEN_REALTIME_THREADS:
    case _SC_XOPEN_SHM:
    case _SC_XOPEN_STREAMS:
    case _SC_XOPEN_UNIX:
    case _SC_XOPEN_UUCP:
      return -1;

    // Options for limits that are indeterminate.
    case _SC_AIO_LISTIO_MAX:
    case _SC_AIO_MAX:
    case _SC_AIO_PRIO_DELTA_MAX:
    case _SC_ARG_MAX:
    case _SC_ATEXIT_MAX:
    case _SC_BC_BASE_MAX:
    case _SC_BC_DIM_MAX:
    case _SC_BC_SCALE_MAX:
    case _SC_BC_STRING_MAX:
    case _SC_CHILD_MAX:
    case _SC_COLL_WEIGHTS_MAX:
    case _SC_DELAYTIMER_MAX:
    case _SC_EXPR_NEST_MAX:
    case _SC_GETGR_R_SIZE_MAX:
    case _SC_GETPW_R_SIZE_MAX:
    case _SC_HOST_NAME_MAX:
    case _SC_IOV_MAX:
    case _SC_LINE_MAX:
    case _SC_LOGIN_NAME_MAX:
    case _SC_MQ_OPEN_MAX:
    case _SC_MQ_PRIO_MAX:
    case _SC_NGROUPS_MAX:
    case _SC_OPEN_MAX:
    case _SC_RTSIG_MAX:
    case _SC_SEM_NSEMS_MAX:
    case _SC_SIGQUEUE_MAX:
    case _SC_SS_REPL_MAX:
    case _SC_STREAM_MAX:
    case _SC_SYMLOOP_MAX:
    case _SC_THREAD_KEYS_MAX:
    case _SC_THREAD_THREADS_MAX:
    case _SC_TIMER_MAX:
    case _SC_TRACE_EVENT_NAME_MAX:
    case _SC_TRACE_NAME_MAX:
    case _SC_TRACE_SYS_MAX:
    case _SC_TRACE_USER_EVENT_MAX:
    case _SC_TTY_NAME_MAX:
    case _SC_TZNAME_MAX:
      return -1;

    // Options whose values are based on limits.
    case _SC_CLK_TCK:
      return CLOCKS_PER_SEC;
    case _SC_RE_DUP_MAX:
      return RE_DUP_MAX;
    case _SC_SEM_VALUE_MAX:
      return SEM_VALUE_MAX;
    case _SC_THREAD_DESTRUCTOR_ITERATIONS:
      return PTHREAD_DESTRUCTOR_ITERATIONS;
    case _SC_THREAD_STACK_MIN:
      return PTHREAD_STACK_MIN;

    // Options whose values are reported by the system.
    case _SC_NPROCESSORS_CONF:
    case _SC_NPROCESSORS_ONLN:
      return __at_ncpus;
    case _SC_PAGESIZE:
      return __at_pagesz;

    default:
      // Unknown option.
      errno = EINVAL;
      return -1;
  }
}
