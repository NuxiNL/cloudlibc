// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

#include "uv_impl.h"

int uv_tcp_open(uv_tcp_t *handle, uv_os_sock_t sock) {
  return __uv_stream_open((uv_stream_t *)handle, sock);
}
