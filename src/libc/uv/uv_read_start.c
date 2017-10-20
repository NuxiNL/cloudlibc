// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <uv.h>

#include "uv_impl.h"

int uv_read_start(uv_stream_t *stream, uv_alloc_cb alloc_cb,
                  uv_read_cb read_cb) {
  if (stream->__fd < 0)
    return UV_EBADF;
  __uv_stream_start_reading(stream);
  stream->__alloc_cb = alloc_cb;
  assert(read_cb != NULL && "Missing required argument alloc_cb");
  stream->__read_cb = read_cb;
  return 0;
}
