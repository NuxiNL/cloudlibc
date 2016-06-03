// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "socket_impl.h"

struct cmsghdr *CMSG_NXTHDR(const struct msghdr *mhdr,
                            const struct cmsghdr *cmsg) {
  // RFC 2292:if the value of the cmsg pointer is NULL, a pointer to
  // the cmsghdr structure describing the first ancillary data object
  // is returned.
  if (cmsg == NULL)
    return CMSG_GET(mhdr, (uintptr_t)mhdr->msg_controllen);

  // Safety belt: CMSG_NXTHDR() should not be called on a malformed
  // control message header. This could cause an infinite loop if
  // cmsg_len were zero.
  assert(cmsg->cmsg_len >= offsetof(struct cmsghdr, __cmsg_data) &&
         "Message header has negative data length");
  return CMSG_GET(mhdr, (uintptr_t)cmsg + cmsg->cmsg_len);
}
