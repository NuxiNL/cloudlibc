// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <testing.h>
#include <wchar.h>

TEST(mbrlen, euro) {
  // Parse all bytes of a Euro symbol separately.
  char euro[] = "€";
  mbstate_t mbs = {};
  ASSERT_EQ((size_t)-2, mbrlen_l(&euro[0], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ((size_t)-2, mbrlen_l(&euro[1], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(1, mbrlen_l(&euro[2], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbrlen_l(&euro[3], 1, &mbs, LC_C_UNICODE_LOCALE));
}
