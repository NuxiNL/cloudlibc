// Copyright (c) 2016-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <arpa/inet.h>

#include <stddef.h>

#include "gtest/gtest.h"

TEST(inet_aton_test, bad) {
  ASSERT_EQ(0, inet_aton("hello", NULL));
  ASSERT_EQ(0, inet_aton("0x100000000", NULL));
  ASSERT_EQ(0, inet_aton("0.0.0.256", NULL));
  ASSERT_EQ(0, inet_aton("0.0.0.0.0", NULL));
  ASSERT_EQ(0, inet_aton("256.0.0.0", NULL));
}

TEST(inet_aton_examples, bad) {
#define TEST_INET_ATON(input, output)                                \
  do {                                                               \
    struct in_addr in;                                               \
    ASSERT_EQ(1, inet_aton(input, &in));                             \
    char out[INET_ADDRSTRLEN];                                       \
    ASSERT_STREQ(output, inet_ntop(AF_INET, &in, out, sizeof(out))); \
  } while (0)
  TEST_INET_ATON("127.0.0.1", "127.0.0.1");
  TEST_INET_ATON("0xffffffff", "255.255.255.255");
  TEST_INET_ATON("012.34.5678", "10.34.22.46");
#undef TEST_INET_ATON
}
