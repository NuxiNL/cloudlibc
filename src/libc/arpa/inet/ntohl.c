// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <stdint.h>

uint32_t ntohl(uint32_t netlong) {
  union {
    uint32_t net;
    uint8_t host[4];
  } v = {.net = netlong};
  return (uint32_t)v.host[0] << 24 | (uint32_t)v.host[1] << 16 |
         (uint32_t)v.host[2] << 8 | v.host[3];
}
