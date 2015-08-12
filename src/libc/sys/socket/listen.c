// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

int listen(int socket, int backlog) {
  if (backlog < 0)
    backlog = 0;
  cloudabi_errno_t error = cloudabi_sys_sock_listen(socket, backlog);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }
  return error;
}
