// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <errno.h>
#include <time.h>

int clock_getres(clockid_t clock_id, struct timespec *res) {
  cloudabi_timestamp_t ts;
  cloudabi_errno_t error = cloudabi_sys_clock_res_get(clock_id, &ts);
  if (error != 0) {
    errno = error;
    return -1;
  }
  *res = timestamp_to_timespec(ts);
  return 0;
}
