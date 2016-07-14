// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include <sys/time.h>

#include <cloudabi_syscalls.h>

int gettimeofday(struct timeval *restrict tp, ...) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, 1000, &ts);
  *tp = timestamp_to_timeval(ts);
  return 0;
}
