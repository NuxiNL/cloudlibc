// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_timer_stop(uv_timer_t *handle) {
  if (uv_is_active((uv_handle_t *)handle)) {
    __uv_active_timers_remove(&handle->loop->__active_timers, handle);
    __uv_handle_stop((uv_handle_t *)handle);
  }
  return 0;
}
