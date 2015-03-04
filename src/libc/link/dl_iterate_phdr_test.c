// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <link.h>
#include <stddef.h>
#include <testing.h>

static int callback(struct dl_phdr_info *pinfo, size_t len, void *data) {
  int *invocation = data;
  ASSERT_EQ(0, *invocation);
  ++*invocation;

  // Validate values that this implementation should provide.
  ASSERT_EQ(0x0, pinfo->dlpi_addr);
  ASSERT_STREQ("unknown", pinfo->dlpi_name);
  ASSERT_NE(NULL, pinfo->dlpi_phdr);
  ASSERT_LT(0, pinfo->dlpi_phnum);
  ASSERT_EQ(1, pinfo->dlpi_adds);
  ASSERT_EQ(0, pinfo->dlpi_subs);
  ASSERT_EQ(1, pinfo->dlpi_tls_modid);
  ASSERT_NE(NULL, pinfo->dlpi_tls_data);

  return 42;
}

TEST(dl_iterate_phdr, example) {
  // In this environment the callback should be invoked exactly once.
  int invocation = 0;
  ASSERT_EQ(42, dl_iterate_phdr(callback, &invocation));
  ASSERT_EQ(1, invocation);
}
