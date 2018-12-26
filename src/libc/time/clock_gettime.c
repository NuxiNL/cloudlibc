// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/clock.h>
#include <common/time.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <time.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_clock_gettime(clockid_t clock_id, struct timespec *tp) {
  cloudabi_timestamp_t ts;
  cloudabi_errno_t error = cloudabi_sys_clock_time_get(clock_id->id, 1, &ts);
  if (error != 0) {
    errno = error;
    return -1;
  }
  *tp = timestamp_to_timespec(ts);
  return 0;
}

__weak_reference(__cloudlibc_clock_gettime, clock_gettime);
