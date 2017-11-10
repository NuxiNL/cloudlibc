// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
