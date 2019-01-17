// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_ip6_addr, einval) {
  struct sockaddr_in6 out;
  ASSERT_EQ(UV_EINVAL, uv_ip6_addr("Invalid address", 12345, &out));
}

TEST(uv_ip6_addr, ok) {
  struct sockaddr_in6 out;
  ASSERT_EQ(0, uv_ip6_addr("2001:67c:2e8:22::c100:68b", 12345, &out));
  ASSERT_EQ(AF_INET6, out.sin6_family);
  ASSERT_EQ(0x20, out.sin6_addr.s6_addr[0]);
  ASSERT_EQ(0x01, out.sin6_addr.s6_addr[1]);
  ASSERT_EQ(0x06, out.sin6_addr.s6_addr[2]);
  ASSERT_EQ(0x7c, out.sin6_addr.s6_addr[3]);
  ASSERT_EQ(0x02, out.sin6_addr.s6_addr[4]);
  ASSERT_EQ(0xe8, out.sin6_addr.s6_addr[5]);
  ASSERT_EQ(0x00, out.sin6_addr.s6_addr[6]);
  ASSERT_EQ(0x22, out.sin6_addr.s6_addr[7]);
  ASSERT_EQ(0x00, out.sin6_addr.s6_addr[8]);
  ASSERT_EQ(0x00, out.sin6_addr.s6_addr[9]);
  ASSERT_EQ(0x00, out.sin6_addr.s6_addr[10]);
  ASSERT_EQ(0x00, out.sin6_addr.s6_addr[11]);
  ASSERT_EQ(0xc1, out.sin6_addr.s6_addr[12]);
  ASSERT_EQ(0x00, out.sin6_addr.s6_addr[13]);
  ASSERT_EQ(0x06, out.sin6_addr.s6_addr[14]);
  ASSERT_EQ(0x8b, out.sin6_addr.s6_addr[15]);
  ASSERT_EQ(htons(12345), out.sin6_port);
}
