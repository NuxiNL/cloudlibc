// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include "gtest/gtest.h"

TEST(inet_ntop, inet_nospace) {
  uint8_t addr[4] = {0, 0, 0, 0};
  ASSERT_EQ(NULL, inet_ntop(AF_INET, &addr, NULL, 7));
  ASSERT_EQ(ENOSPC, errno);
}

TEST(inet_ntop, inet_null) {
  uint8_t addr[4] = {0, 0, 0, 0};
  char buf[sizeof("0.0.0.0")];
  ASSERT_EQ(buf, inet_ntop(AF_INET, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("0.0.0.0", buf);
}

TEST(inet_ntop, inet_localhost) {
  uint8_t addr[4] = {127, 0, 0, 1};
  char buf[sizeof("127.0.0.1")];
  ASSERT_EQ(buf, inet_ntop(AF_INET, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("127.0.0.1", buf);
}

TEST(inet_ntop, inet_longest) {
  uint8_t addr[4] = {255, 255, 255, 255};
  char buf[sizeof("255.255.255.255")];
  ASSERT_EQ(buf, inet_ntop(AF_INET, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("255.255.255.255", buf);
}

TEST(inet_ntop, inet6_rfc2373_example_1) {
  uint8_t addr[16] = {0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x08, 0x08, 0x00, 0x20, 0x0c, 0x41, 0x7a};
  char buf[sizeof("1080::8:800:200c:417a")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("1080::8:800:200c:417a", buf);
}

TEST(inet_ntop, inet6_rfc2373_example_2) {
  uint8_t addr[16] = {0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01};
  char buf[sizeof("ff01::101")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("ff01::101", buf);
}

TEST(inet_ntop, inet6_rfc2373_example_3) {
  char buf[sizeof("::1")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &in6addr_loopback, buf, sizeof(buf)));
  ASSERT_STREQ("::1", buf);
}

TEST(inet_ntop, inet6_rfc2373_example_4) {
  uint8_t addr[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char buf[sizeof("::")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("::", buf);
}

TEST(inet_ntop, inet6_rfc2373_example_5) {
  uint8_t addr[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x0d, 0x01, 0x44, 0x03};
  char buf[sizeof("::13.1.68.3")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("::13.1.68.3", buf);
}

TEST(inet_ntop, inet6_rfc2373_example_6) {
  uint8_t addr[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0xff, 0xff, 0x81, 0x90, 0x34, 0x26};
  char buf[sizeof("::ffff:129.144.52.38")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("::ffff:129.144.52.38", buf);
}

TEST(inet_ntop, inet6_highest) {
  uint8_t addr[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  char buf[sizeof("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff")];
  ASSERT_EQ(buf, inet_ntop(AF_INET6, &addr, buf, sizeof(buf)));
  ASSERT_STREQ("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", buf);
}
