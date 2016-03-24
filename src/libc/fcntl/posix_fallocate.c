// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <fcntl.h>

int posix_fallocate(int fd, off_t offset, off_t len) {
  if (offset < 0 || len < 0)
    return EINVAL;
  return cloudabi_sys_file_allocate(fd, offset, len);
}
