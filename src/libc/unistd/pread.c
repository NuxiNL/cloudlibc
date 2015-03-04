// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <unistd.h>

ssize_t pread(int fildes, void *buf, size_t nbyte, off_t offset) {
  if (offset < 0) {
    errno = EINVAL;
    return -1;
  }
  cloudabi_iovec_t iov = {.iov_base = buf, .iov_len = nbyte};
  cloudabi_size_t bytes_read;
  cloudabi_errno_t error =
      cloudabi_sys_fd_pread(fildes, &iov, 1, offset, &bytes_read);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return bytes_read;
}
