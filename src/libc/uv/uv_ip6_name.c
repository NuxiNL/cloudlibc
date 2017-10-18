// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <netinet/in.h>

#include <uv.h>

int uv_ip6_name(const struct sockaddr_in6 *src, char *dst, size_t size) {
  return uv_inet_ntop(AF_INET6, &src->sin6_addr, dst, size);
}
