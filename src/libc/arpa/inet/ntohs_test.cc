// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include "gtest/gtest.h"

TEST(ntohs, example) {
  union {
    uint32_t host;
    unsigned char net[2];
  } value = {.net = {0x13, 0x37}};
  ASSERT_EQ(0x1337, ntohs(value.host));
}
