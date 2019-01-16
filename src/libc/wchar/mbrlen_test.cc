// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(mbrlen, euro) {
  // Parse all bytes of a Euro symbol separately.
  char euro[] = "€";
  mbstate_t mbs{};
  ASSERT_EQ((size_t)-2, mbrlen_l(&euro[0], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ((size_t)-2, mbrlen_l(&euro[1], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(1, mbrlen_l(&euro[2], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbrlen_l(&euro[3], 1, &mbs, LC_C_UNICODE_LOCALE));
}
