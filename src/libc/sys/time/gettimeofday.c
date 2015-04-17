// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <sys/time.h>

int gettimeofday(struct timeval *restrict tp, ...) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, 1000, &ts);
  tp->tv_sec = ts / NSEC_PER_SEC;
  tp->tv_usec = (ts % NSEC_PER_SEC) / 1000;
  return 0;
}
