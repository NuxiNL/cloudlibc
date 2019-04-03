// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <uv.h>

void uv_update_time(uv_loop_t *loop) {
  cloudabi_timestamp_t ts;
  (void)cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_MONOTONIC, 1000000, &ts);
  loop->__now = ts / 1000000;
}
