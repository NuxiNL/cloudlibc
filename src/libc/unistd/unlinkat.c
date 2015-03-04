// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int unlinkat(int fd, const char *path, int flag) {
  cloudabi_ulflags_t ulflags = 0;
  if ((flag & AT_REMOVEDIR) != 0)
    ulflags |= CLOUDABI_UNLINK_REMOVEDIR;
  cloudabi_errno_t error =
      cloudabi_sys_file_unlink(fd, path, strlen(path), ulflags);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return 0;
}
