// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <time.h>

time_t time(time_t *tloc) {
  cloudabi_timestamp_t ts = 0;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, NSEC_PER_SEC, &ts);
  if (tloc != NULL)
    *tloc = ts / NSEC_PER_SEC;
  return ts / NSEC_PER_SEC;
}
