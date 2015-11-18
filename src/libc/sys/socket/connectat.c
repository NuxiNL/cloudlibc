// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>
#include <string.h>

int connectat(int s, int fd, const char *path) {
  cloudabi_errno_t error = cloudabi_sys_sock_connect(s, fd, path, strlen(path));
  if (error != 0) {
    errno = errno_fixup_directory(fd, errno_fixup_socket(s, error));
    return -1;
  }
  return 0;
}
