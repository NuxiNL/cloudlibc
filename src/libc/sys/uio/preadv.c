// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/types.h>
#include <sys/uio.h>

#include <errno.h>

ssize_t preadv(int fildes, const struct iovec *iov, int iovcnt, off_t offset) {
  if (iovcnt < 0 || offset < 0) {
    errno = EINVAL;
    return -1;
  }
  cloudabi_size_t bytes_read;
  cloudabi_errno_t error = cloudabi_sys_fd_pread(
      fildes, (const cloudabi_iovec_t *)iov, iovcnt, offset, &bytes_read);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return bytes_read;
}
