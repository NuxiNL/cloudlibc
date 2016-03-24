// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/uio.h>

#include <assert.h>
#include <errno.h>
#include <stddef.h>

static_assert(offsetof(struct iovec, iov_base) ==
                  offsetof(cloudabi_ciovec_t, iov_base),
              "Offset mismatch");
static_assert(sizeof(((struct iovec *)0)->iov_base) ==
                  sizeof(((cloudabi_ciovec_t *)0)->iov_base),
              "Size mismatch");
static_assert(offsetof(struct iovec, iov_len) ==
                  offsetof(cloudabi_ciovec_t, iov_len),
              "Offset mismatch");
static_assert(sizeof(((struct iovec *)0)->iov_len) ==
                  sizeof(((cloudabi_ciovec_t *)0)->iov_len),
              "Size mismatch");
static_assert(sizeof(struct iovec) == sizeof(cloudabi_ciovec_t),
              "Size mismatch");

ssize_t writev(int fildes, const struct iovec *iov, int iovcnt) {
  if (iovcnt < 0) {
    errno = EINVAL;
    return -1;
  }
  size_t bytes_written;
  cloudabi_errno_t error = cloudabi_sys_fd_write(
      fildes, (const cloudabi_ciovec_t *)iov, iovcnt, &bytes_written);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return bytes_written;
}
