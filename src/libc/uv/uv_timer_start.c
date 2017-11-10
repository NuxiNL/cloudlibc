// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_timer_start(uv_timer_t *handle, uv_timer_cb cb, uint64_t timeout,
                   uint64_t repeat) {
  uv_loop_t *loop = handle->loop;
  handle->__cb = cb;
  handle->__timeout = loop->__now + timeout;
  handle->__repeat = repeat;
  if (uv_is_active((uv_handle_t *)handle)) {
    // Timer was already running, but the timeout has changed.
    // Update the timer's position in the heap.
    __uv_active_timers_changed(&loop->__active_timers, handle);
  } else {
    // Timer wasn't running yet. Insert it into the heap.
    if (!__uv_active_timers_insert(&loop->__active_timers, handle))
      return UV_ENOMEM;
    __uv_handle_start((uv_handle_t *)handle);
  }
  return 0;
}
