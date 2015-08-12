// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <string.h>

int connectat(int s, int fd, const char *path) {
  cloudabi_errno_t error = cloudabi_sys_sock_connect(s, fd, path, strlen(path));
  return errno_fixup_directory(fd, errno_fixup_socket(s, error));
}
