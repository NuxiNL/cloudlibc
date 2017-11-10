// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_idle_init(uv_loop_t *loop, uv_idle_t *idle) {
  __uv_handle_init(loop, (uv_handle_t *)idle, UV_IDLE);
  return 0;
}
