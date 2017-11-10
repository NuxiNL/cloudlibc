// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <sys/stat.h>

#include <errno.h>
#include <uv.h>

static ssize_t do_fstat(uv_fs_t *req) {
  struct stat sb;
  if (fstat(req->__arguments.__fstat.__file, &sb) != 0)
    return -errno;
  req->statbuf = (uv_stat_t){
      .st_dev = sb.st_dev,
      .st_mode = sb.st_mode,
      .st_nlink = sb.st_nlink,
      .st_ino = sb.st_ino,
      .st_size = sb.st_size,
      .st_atim.tv_sec = sb.st_atim.tv_sec,
      .st_atim.tv_nsec = sb.st_atim.tv_nsec,
      .st_mtim.tv_sec = sb.st_mtim.tv_sec,
      .st_mtim.tv_nsec = sb.st_mtim.tv_nsec,
      .st_ctim.tv_sec = sb.st_ctim.tv_sec,
      .st_ctim.tv_nsec = sb.st_ctim.tv_nsec,
  };
  return 0;
}

int uv_fs_fstat(uv_loop_t *loop, uv_fs_t *req, uv_file file, uv_fs_cb cb) {
  req->__arguments.__fstat.__file = file;
  return __uv_fs_execute(loop, req, UV_FS_FSTAT, do_fstat, cb);
}
