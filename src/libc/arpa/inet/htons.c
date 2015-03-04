// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <stdint.h>

uint16_t htons(uint16_t hostshort) {
  union {
    uint8_t host[2];
    uint16_t net;
  } v = {.host = {hostshort >> 8, hostshort}};
  return v.net;
}
