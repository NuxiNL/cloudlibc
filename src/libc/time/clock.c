// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <time.h>

#define PRECISION (1000000000 / CLOCKS_PER_SEC)

clock_t clock(void) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOCK_PROCESS_CPUTIME_ID, PRECISION, &ts);
  return ts / PRECISION;
}
