// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <uv.h>

int uv_ip4_addr(const char *ip, int port, struct sockaddr_in *addr) {
  *addr = (struct sockaddr_in){
      .sin_family = AF_INET,
      .sin_port = htons(port),
  };
  return uv_inet_pton(AF_INET, ip, &addr->sin_addr);
}
