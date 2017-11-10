// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <cloudabi_syscalls.h>
#include <uv.h>

static ssize_t do_ftruncate(uv_fs_t *req) {
  if (req->__arguments.__ftruncate.__offset < 0)
    return UV_EINVAL;
  cloudabi_filestat_t fs = {.st_size = req->__arguments.__ftruncate.__offset};
  return -cloudabi_sys_file_stat_fput(req->__arguments.__ftruncate.__file, &fs,
                                      CLOUDABI_FILESTAT_SIZE);
}

int uv_fs_ftruncate(uv_loop_t *loop, uv_fs_t *req, uv_file file, int64_t offset,
                    uv_fs_cb cb) {
  req->__arguments.__ftruncate.__file = file;
  req->__arguments.__ftruncate.__offset = offset;
  return __uv_fs_execute(loop, req, UV_FS_FTRUNCATE, do_ftruncate, cb);
}
