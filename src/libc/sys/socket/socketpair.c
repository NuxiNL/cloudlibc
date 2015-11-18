// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

#include "socket_impl.h"

int socketpair(int domain, int type, int protocol, int *socket_vector) {
  // We can only allocate UNIX socket pairs. Validate the parameters in
  // userspace.
  {
    int error = is_unix_socket(domain, type, protocol);
    if (error != 0) {
      errno = error;
      return -1;
    }
  }

  // Create socket pair.
  cloudabi_fd_t fd1, fd2;
  cloudabi_errno_t error = cloudabi_sys_fd_create2(type, &fd1, &fd2);
  if (error != 0) {
    errno = error;
    return -1;
  }
  socket_vector[0] = fd1;
  socket_vector[1] = fd2;
  return 0;
}
