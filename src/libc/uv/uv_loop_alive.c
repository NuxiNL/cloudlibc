// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/uv.h>

#include <uv.h>

int uv_loop_alive(const uv_loop_t *loop) {
  return loop->__active_ref_handles_reqs > 0 ||
         !__uv_closing_handles_empty(&loop->__closing_handles);
}
