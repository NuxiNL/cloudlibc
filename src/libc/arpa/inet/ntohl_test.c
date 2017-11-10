// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include <testing.h>

TEST(ntohl, example) {
  union {
    uint32_t host;
    char net[4];
  } value = {.net = "\xca\xfe\xba\xbe"};
  ASSERT_EQ(0xcafebabe, ntohl(value.host));
}
