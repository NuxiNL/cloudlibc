// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <wchar.h>

#include "gtest/gtest.h"

TEST(mbrtowc, euro) {
  // Parse all bytes of a Euro symbol separately.
  char euro[] = "€";
  mbstate_t mbs{};
  wchar_t wc = 123;
  ASSERT_EQ((size_t)-2, mbrtowc_l(&wc, &euro[0], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(123, wc);
  ASSERT_EQ((size_t)-2, mbrtowc_l(&wc, &euro[1], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(123, wc);
  ASSERT_EQ(1, mbrtowc_l(&wc, &euro[2], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(L'€', wc);
  ASSERT_EQ(0, mbrtowc_l(&wc, &euro[3], 1, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(L'\0', wc);
}
