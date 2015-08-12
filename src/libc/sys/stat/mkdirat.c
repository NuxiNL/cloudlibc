// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/stat.h>

#include <errno.h>
#include <string.h>

int mkdirat(int fd, const char *path, ...) {
  cloudabi_errno_t error = cloudabi_sys_file_create(
      fd, path, strlen(path), CLOUDABI_FILETYPE_DIRECTORY);
  if (error != 0) {
    errno = errno_fixup_directory(fd, error);
    return -1;
  }
  return 0;
}
