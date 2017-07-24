// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef SYS_SOCKET_SOCKET_IMPL_H
#define SYS_SOCKET_SOCKET_IMPL_H

#include <sys/socket.h>
#include <sys/un.h>

#include <netinet/in.h>

#include <assert.h>
#include <cloudabi_types.h>
#include <errno.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdint.h>

static_assert(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr),
              "struct sockaddr_storage too small");
static_assert(alignof(struct sockaddr_storage) == alignof(struct sockaddr),
              "struct sockaddr alignment incorrect");
static_assert(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr_in),
              "struct sockaddr_storage too small");
static_assert(alignof(struct sockaddr_storage) == alignof(struct sockaddr_in),
              "struct sockaddr_in alignment incorrect");
static_assert(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr_in6),
              "struct sockaddr_storage too small");
static_assert(alignof(struct sockaddr_storage) == alignof(struct sockaddr_in6),
              "struct sockaddr_in6 alignment incorrect");
static_assert(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr_un),
              "struct sockaddr_storage too small");
static_assert(alignof(struct sockaddr_storage) == alignof(struct sockaddr_un),
              "struct sockaddr_un alignment incorrect");

static_assert(SOCK_DGRAM == CLOUDABI_FILETYPE_SOCKET_DGRAM, "Value mismatch");
static_assert(SOCK_STREAM == CLOUDABI_FILETYPE_SOCKET_STREAM, "Value mismatch");

// Returns whether a socket domain, type and protocol correspond to a
// valid UNIX socket.
static inline int is_unix_socket(int domain, int type, int protocol) {
  // AF_UNIX indicates a UNIX socket.
  if (domain != AF_UNIX)
    return EAFNOSUPPORT;

  // Valid socket types.
  switch (type) {
    case SOCK_DGRAM:
    case SOCK_STREAM:
      break;
    default:
      return EPROTOTYPE;
  }

  // Protocol number must be zero.
  if (protocol != 0)
    return EPROTONOSUPPORT;

  return 0;
}

// Returns the control message header stored at a provided memory
// address, ensuring that it is stored within the ancillary data buffer
// of a message header.
static inline struct cmsghdr *CMSG_GET(const struct msghdr *mhdr, void *cmsg) {
  // Safety belt: require that the returned object is properly aligned.
  assert((uintptr_t)cmsg % alignof(struct cmsghdr) == 0 &&
         "Attempted to access unaligned control message header");

  // Safety belt: the computed starting address of the control message
  // header may only lie inside the ancillary data buffer, or right
  // after it in case we've reached the end of the buffer.
  const unsigned char *begin = mhdr->msg_control;
  const unsigned char *end = begin + mhdr->msg_controllen;
  assert((unsigned char *)cmsg >= begin &&
         (unsigned char *)cmsg < end + alignof(struct cmsghdr) &&
         "Computed object outside of buffer boundaries");

  // Only return the control message header in case all of its fields
  // lie within the ancillary data buffer.
  return CMSG_DATA((struct cmsghdr *)cmsg) <= end ? cmsg : NULL;
}

#endif
