// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dlfcn.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"

TEST(dladdr, nonexistent) {
  ASSERT_FALSE(dladdr(NULL, NULL));
}

TEST(dladdr, global_function) {
  Dl_info info;
  ASSERT_TRUE(dladdr(reinterpret_cast<void *>(strlen), &info));
  ASSERT_STREQ("unknown", info.dli_fname);
  ASSERT_EQ(0x0, (uintptr_t)info.dli_fbase % sysconf(_SC_PAGESIZE));
  ASSERT_STREQ("strlen", info.dli_sname);
  ASSERT_EQ(reinterpret_cast<void *>(strlen), info.dli_saddr);
  ASSERT_LT(info.dli_fbase, info.dli_saddr);
}

TEST(dladdr, global_variable) {
  // Both entries of the tzname array should match the same symbol.
  Dl_info info;
  ASSERT_TRUE(dladdr(&tzname[0], &info));
  ASSERT_STREQ("unknown", info.dli_fname);
  ASSERT_EQ(0x0, (uintptr_t)info.dli_fbase % sysconf(_SC_PAGESIZE));
  ASSERT_STREQ("tzname", info.dli_sname);
  ASSERT_EQ(static_cast<void *>(tzname), info.dli_saddr);
  ASSERT_LT(info.dli_fbase, info.dli_saddr);

  ASSERT_TRUE(dladdr(&tzname[1], &info));
  ASSERT_STREQ("unknown", info.dli_fname);
  ASSERT_EQ(0x0, (uintptr_t)info.dli_fbase % sysconf(_SC_PAGESIZE));
  ASSERT_STREQ("tzname", info.dli_sname);
  ASSERT_EQ(static_cast<void *>(tzname), info.dli_saddr);
  ASSERT_LT(info.dli_fbase, info.dli_saddr);
}

TEST(dladdr, tls_variable) {
  Dl_info info;
  ASSERT_TRUE(dladdr(&errno, &info));
  ASSERT_STREQ("unknown", info.dli_fname);
  ASSERT_EQ(0x0, (uintptr_t)info.dli_fbase % sysconf(_SC_PAGESIZE));
  ASSERT_STREQ("errno", info.dli_sname);
  ASSERT_EQ(&errno, info.dli_saddr);
}
