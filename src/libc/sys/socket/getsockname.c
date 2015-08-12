// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

#include "socket_impl.h"

int getsockname(int socket, struct sockaddr *restrict address,
                size_t *restrict address_len) {
  // Obtain socket address.
  cloudabi_sockstat_t ss;
  cloudabi_errno_t error = cloudabi_sys_sock_stat_get(socket, &ss, 0);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }

  // Convert socket address to address family specific sockaddr structure.
  *address_len = convert_sockaddr(&ss.ss_sockname, address, *address_len);
  return 0;
}
