// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <netdb.h>
#include <uv.h>

static_assert(sizeof(((uv_getnameinfo_t *)0)->host) == NI_MAXHOST,
              "Field size must match getnameinfo()'s limit");
static_assert(sizeof(((uv_getnameinfo_t *)0)->service) == NI_MAXSERV,
              "Field size must match getnameinfo()'s limit");

int uv_getnameinfo(uv_loop_t *loop, uv_getnameinfo_t *req,
                   uv_getnameinfo_cb getnameinfo_cb,
                   const struct sockaddr *addr, int flags) {
  // TODO(ed): Implement!
  return UV_ENOSYS;
}
