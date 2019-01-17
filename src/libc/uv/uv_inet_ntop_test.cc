// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_inet_ntop, eafnosupport) {
  // Output buffer should remain unmodified upon error.
  ASSERT_EQ(UV_EAFNOSUPPORT, uv_inet_ntop(AF_UNSPEC, NULL, NULL, 128));
}

TEST(uv_inet_ntop, enospc) {
  // Output buffer should remain unmodified upon error.
  in_addr_t in = htonl(0x7f000001);
  ASSERT_EQ(UV_ENOSPC, uv_inet_ntop(AF_INET, &in, NULL, 9));
}

TEST(uv_inet_ntop, ok) {
  in_addr_t in = htonl(0x7f000001);
  char out[10];
  ASSERT_EQ(0, uv_inet_ntop(AF_INET, &in, out, sizeof(out)));
  ASSERT_STREQ("127.0.0.1", out);
}
