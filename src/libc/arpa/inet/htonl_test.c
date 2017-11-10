// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include <testing.h>

TEST(htonl, example) {
  union {
    uint32_t host;
    char net[4];
  } value;
  value.host = htonl(0xcafebabe);
  ASSERT_ARREQ("\xca\xfe\xba\xbe", value.net, __arraycount(value.net));
}
