// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>

#include <sys/socket.h>

#include <cloudabi_syscalls.h>
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
