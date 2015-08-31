// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <assert.h>
#include <time.h>

static_assert(CLOCKS_PER_SEC == NSEC_PER_SEC,
              "Timestamp should need no conversion");

clock_t clock(void) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_PROCESS_CPUTIME_ID, 0, &ts);
  return ts;
}
