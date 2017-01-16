// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>

#include <sys/socket.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <stdint.h>

#include "socket_impl.h"

static_assert(MSG_PEEK == CLOUDABI_MSG_PEEK, "Value mismatch");
static_assert(MSG_WAITALL == CLOUDABI_MSG_WAITALL, "Value mismatch");

ssize_t recvfrom(int socket, void *restrict buffer, size_t length, int flags,
                 struct sockaddr *restrict address,
                 size_t *restrict address_len) {
  // Validate flags.
  if ((flags & ~(MSG_PEEK | MSG_WAITALL)) != 0) {
    errno = EOPNOTSUPP;
    return -1;
  }

  // Prepare input parameters.
  cloudabi_iovec_t iov = {.buf = buffer, .buf_len = length};
  cloudabi_recv_in_t ri = {
      .ri_data = &iov, .ri_data_len = 1, .ri_flags = flags,
  };

  // Perform system call.
  cloudabi_recv_out_t ro;
  cloudabi_errno_t error = cloudabi_sys_sock_recv(socket, &ri, &ro);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }

  // Convert peer address to address family specific sockaddr structure.
  if (address != NULL)
    *address_len = convert_sockaddr(&ro.ro_peername, address, *address_len);
  return ro.ro_datalen;
}
