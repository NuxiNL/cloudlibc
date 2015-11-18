// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

#include "socket_impl.h"

int getpeername(int socket, struct sockaddr *restrict address,
                size_t *restrict address_len) {
  // Obtain peer address.
  cloudabi_sockstat_t ss;
  cloudabi_errno_t error = cloudabi_sys_sock_stat_get(socket, &ss, 0);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }

  // Return ENOTCONN in case we're not connected.
  if (ss.ss_peername.sa_family == CLOUDABI_AF_UNSPEC) {
    errno = ENOTCONN;
    return -1;
  }

  // Convert peer address to address family specific sockaddr structure.
  *address_len = convert_sockaddr(&ss.ss_peername, address, *address_len);
  return 0;
}
