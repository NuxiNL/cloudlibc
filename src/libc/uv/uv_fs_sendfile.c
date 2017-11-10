// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

int uv_fs_sendfile(uv_loop_t *loop, uv_fs_t *req, uv_file out_fd, uv_file in_fd,
                   int64_t in_offset, size_t length, uv_fs_cb cb) {
  // TODO(ed): Implement!
  return UV_ENOSYS;
}
