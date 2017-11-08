// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

int uv_check_init(uv_loop_t *loop, uv_check_t *check) {
  __uv_handle_init(loop, (uv_handle_t *)check, UV_CHECK);
  return 0;
}
