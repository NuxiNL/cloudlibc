// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_tcp_init_ex(uv_loop_t *loop, uv_tcp_t *handle, unsigned int flags) {
  __uv_stream_init(loop, (uv_stream_t *)handle, UV_TCP);
  return 0;
}
