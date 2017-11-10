// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_timer_init(uv_loop_t *loop, uv_timer_t *handle) {
  __uv_handle_init(loop, (uv_handle_t *)handle, UV_TIMER);
  handle->__cb = NULL;
  handle->__repeat = 0;
  return 0;
}
