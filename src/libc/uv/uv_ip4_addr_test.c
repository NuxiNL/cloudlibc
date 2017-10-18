// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <netinet/in.h>

#include <testing.h>
#include <uv.h>

TEST(uv_ip4_addr, einval) {
  struct sockaddr_in out;
  ASSERT_EQ(UV_EINVAL, uv_ip4_addr("Invalid address", 12345, &out));
}

TEST(uv_ip4_addr, ok) {
  struct sockaddr_in out;
  ASSERT_EQ(0, uv_ip4_addr("127.0.0.1", 12345, &out));
  ASSERT_EQ(AF_INET, out.sin_family);
  ASSERT_EQ(htonl(0x7f000001), out.sin_addr.s_addr);
  ASSERT_EQ(htons(12345), out.sin_port);
}
