// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

#include "socket_impl.h"

int socket(int domain, int type, int protocol) {
  // We can only allocate UNIX sockets. Validate the parameters in
  // userspace.
  {
    int error = is_unix_socket(domain, type, protocol);
    if (error != 0) {
      errno = error;
      return -1;
    }
  }

  // Create socket.
  cloudabi_fd_t fd;
  cloudabi_errno_t error = cloudabi_sys_fd_create1(type, &fd);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return fd;
}
