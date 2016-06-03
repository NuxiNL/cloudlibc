// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include "socket_impl.h"

struct cmsghdr *CMSG_FIRSTHDR(const struct msghdr *mhdr) {
  return CMSG_GET(mhdr, mhdr->msg_control);
}
