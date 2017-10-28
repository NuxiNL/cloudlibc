// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

void uv_print_active_handles(uv_loop_t *loop, FILE *stream) {
  struct __uv_handles_head handles;
  __uv_handles_move(&loop->__handles, &handles);
  while (!__uv_handles_empty(&handles)) {
    uv_handle_t *handle = __uv_handles_first(&handles);
    __uv_handles_remove(handle);
    __uv_handles_insert_last(&loop->__handles, handle);
    if (uv_is_active(handle))
      __uv_print_handle(handle, stream);
  }
}