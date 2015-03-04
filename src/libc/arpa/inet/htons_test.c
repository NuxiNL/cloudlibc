// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <testing.h>

TEST(htons, example) {
  union {
    uint32_t host;
    char net[2];
  } value;
  value.host = htons(0x1337);
  ASSERT_ARREQ("\x13\x37", value.net, __arraycount(value.net));
}
