// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

int symlinkat(const char *path1, int fd, const char *path2) {
  cloudabi_errno_t error =
      cloudabi_sys_file_symlink(path1, strlen(path1), fd, path2, strlen(path2));
  if (error != 0) {
    errno = error;
    return -1;
  }
  return 0;
}
