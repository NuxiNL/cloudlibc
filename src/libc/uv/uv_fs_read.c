// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

int uv_fs_read(uv_loop_t *loop, uv_fs_t *req, uv_file file,
               const uv_buf_t *bufs, unsigned int nbufs, int64_t offset,
               uv_fs_cb cb) {
  // TODO(ed): Implement!
  return UV_ENOSYS;
}
