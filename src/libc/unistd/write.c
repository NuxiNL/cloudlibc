// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <cloudlibc_interceptors.h>
#include <errno.h>
#include <unistd.h>

ssize_t __cloudlibc_write(int fildes, const void *buf, size_t nbyte) {
  cloudabi_ciovec_t iov = {.buf = buf, .buf_len = nbyte};
  size_t bytes_written;
  cloudabi_errno_t error =
      cloudabi_sys_fd_write(fildes, &iov, 1, &bytes_written);
  if (error != 0) {
    errno = error == ENOTCAPABLE ? EBADF : error;
    return -1;
  }
  return bytes_written;
}

__weak_reference(__cloudlibc_write, write);
