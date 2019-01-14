// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <link.h>
#include <unistd.h>

#include "gtest/gtest.h"

static int callback(struct dl_phdr_info *pinfo, size_t len, void *data) {
  int *invocation = static_cast<int *>(data);
  EXPECT_EQ(0, *invocation);
  ++*invocation;

  // Validate values that this implementation should provide.
  EXPECT_EQ(0x0, pinfo->dlpi_addr % sysconf(_SC_PAGESIZE));
  EXPECT_STREQ("unknown", pinfo->dlpi_name);
  EXPECT_NE(NULL, pinfo->dlpi_phdr);
  EXPECT_LT(0, pinfo->dlpi_phnum);
  EXPECT_EQ(1, pinfo->dlpi_adds);
  EXPECT_EQ(0, pinfo->dlpi_subs);
  EXPECT_EQ(1, pinfo->dlpi_tls_modid);
  EXPECT_NE(NULL, pinfo->dlpi_tls_data);

  return 42;
}

TEST(dl_iterate_phdr, example) {
  // In this environment the callback should be invoked exactly once.
  int invocation = 0;
  ASSERT_EQ(42, dl_iterate_phdr(callback, &invocation));
  ASSERT_EQ(1, invocation);
}
