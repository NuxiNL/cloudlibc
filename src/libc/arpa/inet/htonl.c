// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <stdint.h>

uint32_t htonl(uint32_t hostlong) {
  union {
    uint8_t host[4];
    uint32_t net;
  } v = {.host = {hostlong >> 24, hostlong >> 16, hostlong >> 8, hostlong}};
  return v.net;
}
