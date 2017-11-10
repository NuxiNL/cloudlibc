// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

void uv_walk(uv_loop_t *loop, uv_walk_cb walk_cb, void *arg) {
  struct __uv_handles_head handles;
  __uv_handles_move(&loop->__handles, &handles);
  while (!__uv_handles_empty(&handles)) {
    uv_handle_t *handle = __uv_handles_first(&handles);
    __uv_handles_remove(handle);
    __uv_handles_insert_last(&loop->__handles, handle);
    walk_cb(handle, arg);
  }
}
