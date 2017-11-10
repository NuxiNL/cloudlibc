// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <cloudabi_syscalls.h>
#include <uv.h>

int uv_async_init(uv_loop_t *loop, uv_async_t *async, uv_async_cb async_cb) {
  // Create a pipe through which we can send data to wake up the loop.
  cloudabi_fd_t readfd, writefd;
  cloudabi_errno_t error = cloudabi_sys_fd_create2(
      CLOUDABI_FILETYPE_SOCKET_STREAM, &readfd, &writefd);
  if (error != 0)
    return -error;

  // Make the pipe non-blocking.
  cloudabi_fdstat_t fds = {.fs_flags = CLOUDABI_FDFLAG_NONBLOCK};
  error = cloudabi_sys_fd_stat_put(readfd, &fds, CLOUDABI_FDSTAT_FLAGS);
  if (error != 0) {
    cloudabi_sys_fd_close(readfd);
    cloudabi_sys_fd_close(writefd);
    return -error;
  }
  error = cloudabi_sys_fd_stat_put(writefd, &fds, CLOUDABI_FDSTAT_FLAGS);
  if (error != 0) {
    cloudabi_sys_fd_close(readfd);
    cloudabi_sys_fd_close(writefd);
    return -error;
  }

  // Initialize and immediately start the loop.
  __uv_handle_init(loop, (uv_handle_t *)async, UV_ASYNC);
  __uv_active_asyncs_insert_last(&async->loop->__active_asyncs, async);
  __uv_handle_start((uv_handle_t *)async);
  async->__readfd = readfd;
  async->__writefd = writefd;
  async->__cb = async_cb;
  return 0;
}
