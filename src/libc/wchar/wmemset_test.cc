// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(wmemset, null) {
  ASSERT_EQ((wchar_t *)5, wmemset((wchar_t *)5, L'A', 0));
}

TEST(wmemset, example) {
  wchar_t buf[11];
  ASSERT_EQ(buf, wmemset(buf, L'!', 10));
  buf[10] = L'\0';
  ASSERT_STREQ(L"!!!!!!!!!!", buf);
}
