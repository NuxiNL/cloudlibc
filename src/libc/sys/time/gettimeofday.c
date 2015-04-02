// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/time.h>

int gettimeofday(struct timeval *restrict tp, ...) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, 1000, &ts);
  tp->tv_sec = ts / 1000000000;
  tp->tv_usec = (ts % 1000000000) / 1000;
  return 0;
}
