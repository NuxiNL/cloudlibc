// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>

#include <sys/socket.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <string.h>

#include "socket_impl.h"

int accept(int socket, struct sockaddr *restrict address,
           size_t *restrict address_len) {
  // Accept incoming connection.
  cloudabi_fd_t fd;
  cloudabi_errno_t error = cloudabi_sys_sock_accept(socket, NULL, &fd);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }

  // This implementation does not keep track of connection metadata.
  // If an address is requested, return AF_UNSPEC.
  if (address != NULL) {
    struct sockaddr sa = {.sa_family = AF_UNSPEC};
    if (*address_len > sizeof(sa))
      *address_len = sizeof(sa);
    memcpy(address, &sa, *address_len);
  }
  return fd;
}
