// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/time.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <time.h>

static_assert(CLOCKS_PER_SEC == NSEC_PER_SEC,
              "Timestamp should need no conversion");

clock_t clock(void) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_PROCESS_CPUTIME_ID, 0, &ts);
  return ts;
}
