// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <netinet/in.h>

#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_inet_pton, eafnosupport) {
  // Output buffer should remain unmodified upon error.
  ASSERT_EQ(UV_EAFNOSUPPORT, uv_inet_pton(AF_UNSPEC, "127.0.0.1", (void *)1));
}

TEST(uv_inet_pton, einval) {
  // Output buffer should remain unmodified upon error.
  ASSERT_EQ(UV_EINVAL, uv_inet_pton(AF_INET, "Invalid address", (void *)1));
}

TEST(uv_inet_pton, ok) {
  in_addr_t out;
  ASSERT_EQ(0, uv_inet_pton(AF_INET, "127.0.0.1", &out));
  ASSERT_EQ(htonl(0x7f000001), out);
}
