// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <netdb.h>

#include "gtest/gtest.h"

TEST(gai_strerror, success) {
  ASSERT_STREQ("Success", gai_strerror(0));
}

TEST(gai_strerror, unknown) {
  ASSERT_STREQ("Unknown error", gai_strerror(-1));
}

TEST(gai_strerror, eai_memory) {
  ASSERT_STREQ("Memory allocation failure", gai_strerror(EAI_MEMORY));
}
