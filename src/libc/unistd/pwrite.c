// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <unistd.h>

ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset) {
  if (offset < 0) {
    errno = EINVAL;
    return -1;
  }
  cloudabi_ciovec_t iov = {.iov_base = buf, .iov_len = nbyte};
  cloudabi_size_t bytes_written;
  cloudabi_errno_t error =
      cloudabi_sys_fd_pwrite(fildes, &iov, 1, offset, &bytes_written);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return bytes_written;
}
