// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>

#include <sys/socket.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <string.h>

int getsockopt(int socket, int level, int option_name,
               void *restrict option_value, size_t *restrict option_len) {
  // Only support SOL_SOCKET options for now.
  if (level != SOL_SOCKET) {
    errno = ENOPROTOOPT;
    return -1;
  }

  int value;
  switch (option_name) {
    case SO_ACCEPTCONN: {
      // Socket is accepting incoming connections.
      cloudabi_sockstat_t ss;
      cloudabi_errno_t error = cloudabi_sys_sock_stat_get(socket, &ss, 0);
      if (error != 0) {
        errno = errno_fixup_socket(socket, error);
        return -1;
      }
      value = (ss.ss_state & CLOUDABI_SOCKSTATE_ACCEPTCONN) != 0;
      break;
    }
    case SO_ERROR: {
      // Fetch and clear socket error state.
      cloudabi_sockstat_t ss;
      cloudabi_errno_t error = cloudabi_sys_sock_stat_get(
          socket, &ss, CLOUDABI_SOCKSTAT_CLEAR_ERROR);
      if (error != 0) {
        errno = errno_fixup_socket(socket, error);
        return -1;
      }
      value = ss.ss_error;
      break;
    }
    case SO_TYPE: {
      // Return the type of the socket. This information can simply be
      // obtained by looking at the file descriptor type.
      cloudabi_fdstat_t fsb;
      if (cloudabi_sys_fd_stat_get(socket, &fsb) != 0) {
        errno = EBADF;
        return -1;
      }
      switch (fsb.fs_filetype) {
        case CLOUDABI_FILETYPE_SOCKET_DGRAM:
        case CLOUDABI_FILETYPE_SOCKET_SEQPACKET:
        case CLOUDABI_FILETYPE_SOCKET_STREAM:
          value = fsb.fs_filetype;
          break;
        default:
          errno = ENOTSOCK;
          return -1;
      }
      break;
    }
    default: {
      errno = ENOPROTOOPT;
      return -1;
    }
  }

  // Copy out integer value.
  memcpy(option_value, &value,
         *option_len < sizeof(int) ? *option_len : sizeof(int));
  *option_len = sizeof(int);
  return 0;
}
