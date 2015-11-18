// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <assert.h>
#include <errno.h>
#include <time.h>

static_assert(CLOCK_MONOTONIC == CLOUDABI_CLOCK_MONOTONIC, "Value mismatch");
static_assert(CLOCK_PROCESS_CPUTIME_ID == CLOUDABI_CLOCK_PROCESS_CPUTIME_ID,
              "Value mismatch");
static_assert(CLOCK_REALTIME == CLOUDABI_CLOCK_REALTIME, "Value mismatch");
static_assert(CLOCK_THREAD_CPUTIME_ID == CLOUDABI_CLOCK_THREAD_CPUTIME_ID,
              "Value mismatch");

int clock_gettime(clockid_t clock_id, struct timespec *tp) {
  cloudabi_timestamp_t ts;
  cloudabi_errno_t error = cloudabi_sys_clock_time_get(clock_id, 1, &ts);
  if (error != 0) {
    errno = error;
    return -1;
  }
  *tp = timestamp_to_timespec(ts);
  return 0;
}
