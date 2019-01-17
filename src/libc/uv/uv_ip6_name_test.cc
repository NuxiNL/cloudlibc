// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_ip6_name, enospc) {
  // Output buffer should remain unmodified upon error.
  struct sockaddr_in6 in = {
      .sin6_family = AF_INET6,
      .sin6_addr.s6_addr = {0x20, 0x01, 0x06, 0x7c, 0x02, 0xe8, 0x00, 0x22,
                            0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x06, 0x8b},
  };
  ASSERT_EQ(UV_ENOSPC, uv_ip6_name(&in, NULL, 25));
}

TEST(uv_ip6_name, ok) {
  struct sockaddr_in6 in = {
      .sin6_family = AF_INET6,
      .sin6_addr.s6_addr = {0x20, 0x01, 0x06, 0x7c, 0x02, 0xe8, 0x00, 0x22,
                            0x00, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x06, 0x8b},
  };
  char out[26];
  ASSERT_EQ(0, uv_ip6_name(&in, out, sizeof(out)));
  ASSERT_STREQ("2001:67c:2e8:22::c100:68b", out);
}
