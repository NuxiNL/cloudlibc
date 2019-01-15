// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <netinet/in.h>

#include <netdb.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(getprotobyname, bad) {
  ASSERT_EQ(NULL, getprotobyname("nonexistent"));
}

TEST(getprotobyname, ipproto_constants) {
  ASSERT_EQ(IPPROTO_ICMP, getprotobyname("icmp")->p_proto);
  ASSERT_EQ(IPPROTO_IP, getprotobyname("ip")->p_proto);
  ASSERT_EQ(IPPROTO_IPV6, getprotobyname("IPv6")->p_proto);
  ASSERT_EQ(IPPROTO_RAW, getprotobyname("raw")->p_proto);
  ASSERT_EQ(IPPROTO_TCP, getprotobyname("TCP")->p_proto);
  ASSERT_EQ(IPPROTO_UDP, getprotobyname("udp")->p_proto);
}
