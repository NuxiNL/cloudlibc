// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>

static_assert(MSG_EOR == CLOUDABI_MSG_EOR, "Value mismatch");

ssize_t send(int socket, const void *buffer, size_t length, int flags) {
  // Validate flags.
  if ((flags & ~(MSG_EOR | MSG_NOSIGNAL)) != 0) {
    errno = EOPNOTSUPP;
    return -1;
  }

  // Prepare input parameters.
  cloudabi_ciovec_t iov = {.iov_base = buffer, .iov_len = length};
  cloudabi_send_in_t si = {
      .si_data = &iov, .si_datalen = 1, .si_flags = flags,
  };

  // Perform system call.
  cloudabi_send_out_t so;
  cloudabi_errno_t error = cloudabi_sys_sock_send(socket, &si, &so);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }
  return so.so_datalen;
}
