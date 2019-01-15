// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(htonl, example) {
  union {
    uint32_t host;
    char net[4];
  } value;
  value.host = htonl(0xcafebabe);
  ASSERT_THAT(value.net, testing::ElementsAreArray({0xca, 0xfe, 0xba, 0xbe}));
}
