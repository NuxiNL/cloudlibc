// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <unistd.h>

ssize_t write(int fildes, const void *buf, size_t nbyte) {
  cloudabi_ciovec_t iov = {.iov_base = buf, .iov_len = nbyte};
  size_t bytes_written;
  cloudabi_errno_t error =
      cloudabi_sys_fd_write(fildes, &iov, 1, &bytes_written);
  if (error != 0) {
    errno = error == ENOTCAPABLE ? EBADF : error;
    return -1;
  }
  return bytes_written;
}
