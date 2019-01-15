// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(htons, example) {
  union {
    uint32_t host;
    char net[2];
  } value;
  value.host = htons(0x1337);
  ASSERT_THAT(value.net, testing::ElementsAreArray({0x13, 0x37}));
}
