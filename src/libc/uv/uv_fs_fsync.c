// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <cloudabi_syscalls.h>
#include <uv.h>

static ssize_t do_fsync(uv_fs_t *req) {
  return -cloudabi_sys_fd_sync(req->__arguments.__fsync.__file);
}

int uv_fs_fsync(uv_loop_t *loop, uv_fs_t *req, uv_file file, uv_fs_cb cb) {
  req->__arguments.__fsync.__file = file;
  return __uv_fs_execute(loop, req, UV_FS_FSYNC, do_fsync, cb);
}
