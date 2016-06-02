// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#ifndef CMSG_NXTHDR
#error "CMSG_NXTHDR is supposed to be a macro as well"
#endif

struct cmsghdr *(CMSG_NXTHDR)(const struct msghdr *mhdr,
                              const struct cmsghdr *cmsg) {
  return CMSG_NXTHDR(mhdr, cmsg);
}
