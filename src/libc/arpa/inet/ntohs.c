// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <stdint.h>

uint16_t ntohs(uint16_t netshort) {
  union {
    uint16_t net;
    uint8_t host[2];
  } v = {.net = netshort};
  return (uint16_t)v.host[0] << 8 | v.host[1];
}
