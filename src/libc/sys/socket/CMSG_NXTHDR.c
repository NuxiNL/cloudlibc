// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <stdint.h>

#include "socket_impl.h"

struct cmsghdr *CMSG_NXTHDR(const struct msghdr *mhdr,
                            const struct cmsghdr *cmsg) {
  return CMSG_GET(mhdr, cmsg == NULL ? (uintptr_t)mhdr->msg_controllen
                                     : (uintptr_t)cmsg + cmsg->cmsg_len);
}
