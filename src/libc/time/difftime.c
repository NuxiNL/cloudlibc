// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/limits.h>

#include <assert.h>
#include <stdint.h>
#include <time.h>

static_assert(sizeof(time_t) == sizeof(uint64_t), "Size mismatch");

double difftime(time_t time1, time_t time0) {
  uint64_t t1 = time1 - NUMERIC_MIN(time_t);
  uint64_t t0 = time0 - NUMERIC_MIN(time_t);
  if (t1 >= t0)
    return (double)(t1 - t0);
  else
    return -(double)(t0 - t1);
}
