// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>
#include <wctype.h>

TEST(wcwidth, iswprint) {
  ASSERT_EQ(0, wcwidth(L'\0'));
  for (wchar_t wc = 1; wc < 0x10ffff; ++wc) {
    SCOPED_NOTE(wc, { ASSERT_EQ(iswprint(wc), wcwidth(wc) >= 0); });
  }
}
