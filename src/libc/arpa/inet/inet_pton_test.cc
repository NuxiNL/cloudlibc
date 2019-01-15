// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <arpa/inet.h>

#include <stddef.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(inet_pton, inet_valid) {
  unsigned char addr[4];
#define TEST_INET(string, ...)                     \
  ASSERT_EQ(1, inet_pton(AF_INET, string, &addr)); \
  ASSERT_THAT(addr, testing::ElementsAreArray(__VA_ARGS__));
  TEST_INET("0.0.0.0", {0, 0, 0, 0});
  TEST_INET("192.168.3.4", {192, 168, 3, 4});
  TEST_INET("255.255.255.255", {255, 255, 255, 255});
#undef TEST_INET
}

TEST(inet_pton, inet_invalid) {
  ASSERT_EQ(0, inet_pton(AF_INET, "256.256.256.256", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "10.0.0.1hello", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "10.0.0.", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "hi10.0.0.5", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "00.0.0.0", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET, "0.0.0.0.0", NULL));
}

TEST(inet_pton, inet6_valid) {
  char addr[16];
#define TEST_INET6(string, ...)                     \
  ASSERT_EQ(1, inet_pton(AF_INET6, string, &addr)); \
  ASSERT_THAT(addr, testing::ElementsAreArray(__VA_ARGS__))
  TEST_INET6("1080::8:800:200c:417a",
             {0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08,
              0x00, 0x20, 0x0c, 0x41, 0x7a});
  TEST_INET6("ff01::101", {0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                           0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01});
  TEST_INET6("::1", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
  TEST_INET6("::", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
  TEST_INET6("2001:DB8:0:1:1:1:1:1",
             {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
              0x01, 0x00, 0x01, 0x00, 0x01});
  TEST_INET6("::13.1.68.3", {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x0d, 0x01, 0x44, 0x03});
  TEST_INET6("0:0:0:0::13.1.68.3",
             {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x0d, 0x01, 0x44, 0x03});
  TEST_INET6("0:0:0:0:0:0:13.1.68.3",
             {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x0d, 0x01, 0x44, 0x03});
  TEST_INET6("::ffff:129.144.52.38",
             {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
              0xff, 0x81, 0x90, 0x34, 0x26});
  TEST_INET6("2001:db8:1:1:1:1::",
             {0x20, 0x01, 0x0d, 0xb8, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
              0x01, 0x00, 0x00, 0x00, 0x00});
#undef TEST_INET6
}

TEST(inet_pton, inet6_invalid) {
  ASSERT_EQ(0, inet_pton(AF_INET6, "", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, ":::", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, ":1:2:3:4:5:6:7", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "1:2:3:4:5:6:7:", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "1::2::3", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "0:0:0:0:0::13.1.68.3", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "0:0:0:0:0:0::13.1.68.3", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "2001:DB8::1:1:1:1:1", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "2001:db8:1:1:1:1:1:", NULL));
  ASSERT_EQ(0, inet_pton(AF_INET6, "2001:db8:1:1:1:1:1::", NULL));
}
