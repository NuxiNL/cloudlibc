// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <unistd.h>

int dup2(int fildes, int fildes2) {
  cloudabi_errno_t error = cloudabi_sys_fd_replace(fildes, fildes2);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return fildes2;
}
