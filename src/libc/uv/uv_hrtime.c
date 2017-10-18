// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <cloudabi_syscalls.h>
#include <uv.h>

uint64_t uv_hrtime(void) {
  cloudabi_timestamp_t ts;
  cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_MONOTONIC, 1, &ts);
  return ts;
}
