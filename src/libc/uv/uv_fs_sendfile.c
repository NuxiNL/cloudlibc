// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

int uv_fs_sendfile(uv_loop_t *loop, uv_fs_t *req, uv_file out_fd, uv_file in_fd,
                   int64_t in_offset, size_t length, uv_fs_cb cb) {
  // TODO(ed): Implement!
  return UV_ENOSYS;
}
