// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <uv.h>

uint64_t uv_hrtime(void) {
  cloudabi_timestamp_t ts;
  (void)cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_MONOTONIC, 1, &ts);
  return ts;
}
