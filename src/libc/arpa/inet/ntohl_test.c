// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <arpa/inet.h>

#include <testing.h>

TEST(ntohl, example) {
  union {
    uint32_t host;
    char net[4];
  } value = {.net = "\xca\xfe\xba\xbe"};
  ASSERT_EQ(0xcafebabe, ntohl(value.host));
}
