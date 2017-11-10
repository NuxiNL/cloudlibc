// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <uv.h>

int uv_async_send(uv_async_t *async) {
  // Write a single byte into the pipe to wake up the event loop.
  // If the write fails with EAGAIN, it means that other async calls
  // are pending, which is good.
  char c = 0;
  cloudabi_ciovec_t iov = {.buf = &c, .buf_len = 1};
  size_t nwritten;
  cloudabi_errno_t error =
      cloudabi_sys_fd_write(async->__writefd, &iov, 1, &nwritten);
  return error == CLOUDABI_EAGAIN ? 0 : -error;
}
