// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdbool.h>
#include <uv.h>

#include "uv_impl.h"

int uv_tcp_init(uv_loop_t *loop, uv_tcp_t *handle) {
  __uv_stream_init(loop, (uv_stream_t *)handle, UV_TCP, false);
  return 0;
}
