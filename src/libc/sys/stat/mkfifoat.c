// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>

#include <sys/stat.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <string.h>

int mkfifoat(int fd, const char *path, ...) {
  cloudabi_errno_t error =
      cloudabi_sys_file_create(fd, path, strlen(path), CLOUDABI_FILETYPE_FIFO);
  if (error != 0) {
    errno = errno_fixup_directory(fd, error);
    return -1;
  }
  return 0;
}
