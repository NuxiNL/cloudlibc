// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/mman.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <fcntl.h>

int shm_open(const char *name, int oflag, ...) {
  // O_CREAT, O_EXCL and O_TRUNC are ignored. O_RDWR has to be set.
  if (name != SHM_ANON || (oflag & ~(O_CREAT | O_EXCL | O_TRUNC)) != O_RDWR) {
    errno = EINVAL;
    return -1;
  }

  // Create new shared memory object.
  cloudabi_fd_t fildes;
  cloudabi_errno_t error =
      cloudabi_sys_fd_create1(CLOUDABI_FILETYPE_SHARED_MEMORY, &fildes);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return fildes;
}
