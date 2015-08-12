// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

ssize_t readlinkat(int fd, const char *restrict path, char *restrict buf,
                   size_t bufsize) {
  cloudabi_size_t bufused;
  cloudabi_errno_t error = cloudabi_sys_file_readlink(fd, path, strlen(path),
                                                      buf, bufsize, &bufused);
  if (error != 0) {
    errno = errno_fixup_directory(fd, error);
    return -1;
  }
  return bufused;
}
