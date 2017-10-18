// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_idle_init(uv_loop_t *loop, uv_idle_t *idle) {
  __uv_handle_init(loop, (uv_handle_t *)idle, UV_IDLE);
  return 0;
}
