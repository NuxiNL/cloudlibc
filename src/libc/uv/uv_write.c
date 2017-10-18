// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <uv.h>

int uv_write(uv_write_t *req, uv_stream_t *handle, const uv_buf_t *bufs,
             unsigned int nbufs, uv_write_cb cb) {
  return uv_write2(req, handle, bufs, nbufs, NULL, cb);
}
