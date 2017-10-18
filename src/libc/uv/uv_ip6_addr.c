// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <netinet/in.h>

#include <uv.h>

int uv_ip6_addr(const char *ip, int port, struct sockaddr_in6 *addr) {
  *addr = (struct sockaddr_in6){
      .sin6_family = AF_INET6,
      .sin6_port = htons(port),
  };
  return uv_inet_pton(AF_INET6, ip, &addr->sin6_addr);
}
