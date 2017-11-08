// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

int uv_prepare_init(uv_loop_t *loop, uv_prepare_t *prepare) {
  __uv_handle_init(loop, (uv_handle_t *)prepare, UV_PREPARE);
  return 0;
}
