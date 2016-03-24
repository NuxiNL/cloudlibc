// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <unistd.h>

int fdatasync(int fildes) {
  cloudabi_errno_t error = cloudabi_sys_fd_datasync(fildes);
  if (error != 0) {
    errno = error == ENOTCAPABLE ? EBADF : error;
    return -1;
  }
  return 0;
}
