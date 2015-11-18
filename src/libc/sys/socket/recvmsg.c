// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/socket.h>

#include <errno.h>
#include <stdint.h>

#include "socket_impl.h"

static_assert(MSG_CTRUNC == CLOUDABI_MSG_CTRUNC, "Value mismatch");
static_assert(MSG_EOR == CLOUDABI_MSG_EOR, "Value mismatch");
static_assert(MSG_PEEK == CLOUDABI_MSG_PEEK, "Value mismatch");
static_assert(MSG_TRUNC == CLOUDABI_MSG_TRUNC, "Value mismatch");
static_assert(MSG_WAITALL == CLOUDABI_MSG_WAITALL, "Value mismatch");

ssize_t recvmsg(int socket, struct msghdr *message, int flags) {
  // Validate flags.
  if ((flags & ~(MSG_PEEK | MSG_WAITALL)) != 0) {
    errno = EOPNOTSUPP;
    return -1;
  }

  // Number of I/O vectors must be positive.
  if (message->msg_iovlen <= 0) {
    errno = EMSGSIZE;
    return -1;
  }

  // Determine how many file descriptors we can store in the provided
  // ancillary data buffer.
  struct cmsghdr *fds_cmsg = CMSG_FIRSTHDR(message);
  unsigned char *fds_begin = CMSG_DATA(fds_cmsg);
  unsigned char *fds_end =
      (unsigned char *)message->msg_control + message->msg_controllen;

  // Prepare input parameters.
  cloudabi_recv_in_t ri = {
      .ri_data = (const cloudabi_iovec_t *)message->msg_iov,
      .ri_datalen = message->msg_iovlen,
      .ri_fds = (cloudabi_fd_t *)fds_begin,
      .ri_fdslen = fds_end >= fds_begin
                       ? (fds_end - fds_begin) / sizeof(cloudabi_fd_t)
                       : 0,
      .ri_flags = flags,
  };

  // Perform system call.
  cloudabi_recv_out_t ro;
  cloudabi_errno_t error = cloudabi_sys_sock_recv(socket, &ri, &ro);
  if (error != 0) {
    errno = errno_fixup_socket(socket, error);
    return -1;
  }
  message->msg_flags = ro.ro_flags;

  if (ro.ro_fdslen > 0) {
    // Call yielded received file descriptors. Add SCM_RIGHTS header
    // before the file descriptors and update msg_controllen.
    fds_cmsg->cmsg_len = CMSG_LEN(ro.ro_fdslen * sizeof(int));
    fds_cmsg->cmsg_level = SOL_SOCKET;
    fds_cmsg->cmsg_type = SCM_RIGHTS;
    message->msg_controllen = (unsigned char *)fds_cmsg + fds_cmsg->cmsg_len -
                              (unsigned char *)message->msg_control;
  } else {
    // No ancillary data to return.
    message->msg_controllen = 0;
  }

  // Convert peer address to address family specific sockaddr structure.
  if (message->msg_name != NULL)
    message->msg_namelen = convert_sockaddr(&ro.ro_peername, message->msg_name,
                                            message->msg_namelen);
  return ro.ro_datalen;
}
