// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

#include "socket_impl.h"

int accept(int socket, struct sockaddr *restrict address,
           size_t *restrict address_len) {
  // Accept incoming connection.
  cloudabi_sockstat_t ss;
  cloudabi_fd_t fd;
  cloudabi_errno_t error =
      cloudabi_sys_sock_accept(socket, address != NULL ? &ss : NULL, &fd);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }

  // Convert peer address to address family specific sockaddr structure.
  if (address != NULL)
    *address_len = convert_sockaddr(&ss.ss_peername, address, *address_len);
  return fd;
}
