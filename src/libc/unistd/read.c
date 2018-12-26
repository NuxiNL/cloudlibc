// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <unistd.h>
#include <cloudlibc_interceptors.h>

ssize_t __cloudlibc_read(int fildes, void *buf, size_t nbyte) {
  cloudabi_iovec_t iov = {.buf = buf, .buf_len = nbyte};
  size_t bytes_read;
  cloudabi_errno_t error = cloudabi_sys_fd_read(fildes, &iov, 1, &bytes_read);
  if (error != 0) {
    errno = error == ENOTCAPABLE ? EBADF : error;
    return -1;
  }
  return bytes_read;
}

__weak_reference(__cloudlibc_read, read);
