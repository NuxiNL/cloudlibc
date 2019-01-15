// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include "gtest/gtest.h"

TEST(ntohl, example) {
  union {
    uint32_t host;
    unsigned char net[4];
  } value = {.net = {0xca, 0xfe, 0xba, 0xbe}};
  ASSERT_EQ(0xcafebabe, ntohl(value.host));
}
