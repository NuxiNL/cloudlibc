// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

int uv_getaddrinfo(uv_loop_t *loop, uv_getaddrinfo_t *req,
                   uv_getaddrinfo_cb getaddrinfo_cb, const char *node,
                   const char *service, const struct addrinfo *hints) {
  // TODO(ed): Implement!
  return UV_ENOSYS;
}
