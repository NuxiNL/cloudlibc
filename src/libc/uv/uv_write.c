// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <uv.h>

int uv_write(uv_write_t *req, uv_stream_t *handle, const uv_buf_t *bufs,
             unsigned int nbufs, uv_write_cb cb) {
  return uv_write2(req, handle, bufs, nbufs, NULL, cb);
}
