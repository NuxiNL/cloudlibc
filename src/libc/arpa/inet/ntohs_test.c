// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <testing.h>

TEST(ntohs, example) {
  union {
    uint32_t host;
    char net[2];
  } value = {.net = "\x13\x37"};
  ASSERT_EQ(0x1337, ntohs(value.host));
}
