// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_ip4_name, enospc) {
  // Output buffer should remain unmodified upon error.
  struct sockaddr_in in = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(0x7f000001),
  };
  ASSERT_EQ(UV_ENOSPC, uv_ip4_name(&in, NULL, 9));
}

TEST(uv_ip4_name, ok) {
  struct sockaddr_in in = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(0x7f000001),
  };
  char out[10];
  ASSERT_EQ(0, uv_ip4_name(&in, out, sizeof(out)));
  ASSERT_STREQ("127.0.0.1", out);
}
