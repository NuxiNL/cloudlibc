// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/time.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <time.h>

static_assert(TIME_UTC > 0, "TIME_UTC has to be greater than 0");
static_assert(TIME_UTC == CLOUDABI_CLOCK_REALTIME, "Value mismatch");

int timespec_get(struct timespec *ts, int base) {
  cloudabi_timestamp_t t;
  cloudabi_errno_t error = cloudabi_sys_clock_time_get(base, 1, &t);
  if (error != 0)
    return 0;
  *ts = timestamp_to_timespec(t);
  return base;
}
