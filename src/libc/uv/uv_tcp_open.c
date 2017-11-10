// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <uv.h>

int uv_tcp_open(uv_tcp_t *handle, uv_os_sock_t sock) {
  return __uv_stream_open((uv_stream_t *)handle, sock);
}
