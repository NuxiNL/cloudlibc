// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <time.h>

static_assert(TIME_UTC > 0, "TIME_UTC has to be greater than 0");
static_assert(TIME_UTC == CLOCK_REALTIME, "Value mismatch");

int timespec_get(struct timespec *ts, int base) {
  if (clock_gettime(base, ts) != 0)
    return 0;
  return base;
}
