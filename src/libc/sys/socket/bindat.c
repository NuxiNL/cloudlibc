// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/socket.h>

#include <string.h>

int bindat(int s, int fd, const char *path) {
  return cloudabi_sys_sock_bind(s, fd, path, strlen(path));
}
