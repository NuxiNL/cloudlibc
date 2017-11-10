// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <uv.h>

int uv_ip4_name(const struct sockaddr_in *src, char *dst, size_t size) {
  return uv_inet_ntop(AF_INET, &src->sin_addr, dst, size);
}
