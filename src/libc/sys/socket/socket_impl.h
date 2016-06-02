// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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
#include <stdint.h>
#include <string.h>

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
static_assert(SOCK_SEQPACKET == CLOUDABI_FILETYPE_SOCKET_SEQPACKET,
              "Value mismatch");
static_assert(SOCK_STREAM == CLOUDABI_FILETYPE_SOCKET_STREAM, "Value mismatch");

static_assert(AF_UNSPEC == CLOUDABI_AF_UNSPEC, "Value mismatch");
static_assert(AF_INET == CLOUDABI_AF_INET, "Value mismatch");
static_assert(AF_INET6 == CLOUDABI_AF_INET6, "Value mismatch");
static_assert(AF_UNIX == CLOUDABI_AF_UNIX, "Value mismatch");

// Returns whether a socket domain, type and protocol correspond to a
// valid UNIX socket.
static inline int is_unix_socket(int domain, int type, int protocol) {
  // AF_UNIX indicates a UNIX socket.
  if (domain != AF_UNIX)
    return EAFNOSUPPORT;

  // Valid socket types.
  switch (type) {
    case SOCK_DGRAM:
    case SOCK_SEQPACKET:
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

// Converts an internal socket address structure to a public socket
// address structure.
static inline size_t convert_sockaddr(const cloudabi_sockaddr_t *in,
                                      struct sockaddr *out, size_t outlen) {
  union {
    struct sockaddr def;
    struct sockaddr_in in;
    struct sockaddr_in6 in6;
    struct sockaddr_un un;
  } ret;
  size_t len;

  switch (in->sa_family) {
    case CLOUDABI_AF_INET: {
      ret.in = (struct sockaddr_in){.sin_family = AF_INET};
      memcpy(&ret.in.sin_addr, in->sa_inet.addr, sizeof(ret.in.sin_addr));
      ret.in.sin_port = htons(in->sa_inet.port);
      len = sizeof(ret.in);
      break;
    }
    case CLOUDABI_AF_INET6: {
      ret.in6 = (struct sockaddr_in6){.sin6_family = AF_INET6};
      memcpy(&ret.in6.sin6_addr, in->sa_inet6.addr, sizeof(ret.in6.sin6_addr));
      ret.in6.sin6_port = htons(in->sa_inet6.port);
      len = sizeof(ret.in6);
      break;
    }
    case CLOUDABI_AF_UNIX: {
      ret.un = (struct sockaddr_un){.sun_family = AF_UNIX};
      len = sizeof(ret.un);
      break;
    }
    default: {
      ret.def = (struct sockaddr){.sa_family = AF_UNSPEC};
      len = sizeof(ret.def);
      break;
    }
  }

  memcpy(out, &ret, len < outlen ? len : outlen);
  return len;
}

// Returns the control message header stored at a provided memory
// address, ensuring that it is stored within the ancillary data buffer
// of a message header.
static inline struct cmsghdr *CMSG_GET(const struct msghdr *mhdr,
                                       uintptr_t addr) {
  // Apply the alignment of the control message header.
  unsigned char *pos =
      (unsigned char *)__roundup(addr, _Alignof(struct cmsghdr));

  // Safety belt: the computed starting address of the control message
  // header may only lie inside the ancillary data buffer, or right
  // after it in case we've reached the end of the buffer.
  const unsigned char *begin = mhdr->msg_control;
  const unsigned char *end = begin + mhdr->msg_controllen;
  assert(pos >= begin && pos < end + _Alignof(struct cmsghdr) &&
         "Computed object outside of buffer boundaries");

  // Only return the control message header in case all of its fields
  // lie within the ancillary data buffer.
  struct cmsghdr *cmsg = (struct cmsghdr *)pos;
  return CMSG_DATA(cmsg) <= end ? cmsg : NULL;
}

#endif
