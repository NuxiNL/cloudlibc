// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <cloudabi_syscalls.h>
#include <uv.h>

static ssize_t do_futime(uv_fs_t *req) {
  cloudabi_filestat_t fs = {
      .st_atim = req->__arguments.__futime.__atime * 1000000000,
      .st_mtim = req->__arguments.__futime.__mtime * 1000000000,
  };
  return -cloudabi_sys_file_stat_fput(
      req->__arguments.__ftruncate.__file, &fs,
      CLOUDABI_FILESTAT_ATIM | CLOUDABI_FILESTAT_MTIM);
}

int uv_fs_futime(uv_loop_t *loop, uv_fs_t *req, uv_file file, double atime,
                 double mtime, uv_fs_cb cb) {
  req->__arguments.__futime.__file = file;
  req->__arguments.__futime.__atime = atime;
  req->__arguments.__futime.__mtime = mtime;
  return __uv_fs_execute(loop, req, UV_FS_FUTIME, do_futime, cb);
}
