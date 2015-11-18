// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

#include <errno.h>
#include <unistd.h>

long sysconf(int name) {
  switch (name) {
    case _SC_2_C_BIND:
    case _SC_ADVISORY_INFO:
    case _SC_ASYNCHRONOUS_IO:
    case _SC_BARRIERS:
    case _SC_CLOCK_SELECTION:
    case _SC_CPUTIME:
    case _SC_FSYNC:
    case _SC_IPV6:
    case _SC_MAPPED_FILES:
    case _SC_MEMLOCK_RANGE:
    case _SC_MEMORY_PROTECTION:
    case _SC_MONOTONIC_CLOCK:
    case _SC_RAW_SOCKETS:
    case _SC_READER_WRITER_LOCKS:
    case _SC_REGEXP:
    case _SC_SEMAPHORES:
    case _SC_SHARED_MEMORY_OBJECTS:
    case _SC_SPIN_LOCKS:
    case _SC_SYNCHRONIZED_IO:
    case _SC_THREADS:
    case _SC_THREAD_ATTR_STACKADDR:
    case _SC_THREAD_ATTR_STACKSIZE:
    case _SC_THREAD_CPUTIME:
    case _SC_THREAD_PRIO_INHERIT:
    case _SC_THREAD_PROCESS_SHARED:
    case _SC_THREAD_SAFE_FUNCTIONS:
    case _SC_TIMEOUTS:
    case _SC_XOPEN_ENH_I18N:
      // Options that are always supported.
      return 200809L;

    case _SC_2_CHAR_TERM:
    case _SC_2_C_DEV:
    case _SC_2_FORT_DEV:
    case _SC_2_FORT_RUN:
    case _SC_2_LOCALEDEF:
    case _SC_2_SW_DEV:
    case _SC_2_UPE:
    case _SC_JOB_CONTROL:
    case _SC_MEMLOCK:
    case _SC_MESSAGE_PASSING:
    case _SC_PRIORITIZED_IO:
    case _SC_PRIORITY_SCHEDULING:
    case _SC_REALTIME_SIGNALS:
    case _SC_SAVED_IDS:
    case _SC_SHELL:
    case _SC_SPAWN:
    case _SC_SPORADIC_SERVER:
    case _SC_THREAD_PRIORITY_SCHEDULING:
    case _SC_THREAD_PRIO_PROTECT:
    case _SC_THREAD_ROBUST_PRIO_INHERIT:
    case _SC_THREAD_ROBUST_PRIO_PROTECT:
    case _SC_THREAD_SPORADIC_SERVER:
    case _SC_TIMERS:
    case _SC_TYPED_MEMORY_OBJECTS:
    case _SC_V7_ILP32_OFF32:
    case _SC_V7_ILP32_OFFBIG:
    case _SC_V7_LP64_OFF64:
    case _SC_V7_LPBIG_OFFBIG:
    case _SC_XOPEN_CRYPT:
    case _SC_XOPEN_REALTIME:
    case _SC_XOPEN_REALTIME_THREADS:
    case _SC_XOPEN_SHM:
    case _SC_XOPEN_UNIX:
    case _SC_XOPEN_UUCP:
      // Options that are always unsupported.
      return -1;

    case _SC_NPROCESSORS_ONLN:
      // Number of processors online.
      return __ncpus;
    case _SC_PAGESIZE:
      // Page size.
      return __pagesize;

    default:
      // Unknown option.
      errno = EINVAL;
      return -1;
  }
}
