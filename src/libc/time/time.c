// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <time.h>

#define PRECISION 1000000000

time_t time(time_t *tloc) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, PRECISION, &ts);
  if (tloc != NULL)
    *tloc = ts / PRECISION;
  return ts / PRECISION;
}
