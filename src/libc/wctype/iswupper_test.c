// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wctype.h>

TEST(iswupper, equality) {
  for (wint_t wc = 0; wc <= 0x10ffff; ++wc) {
    SCOPED_NOTE(wc, { ASSERT_EQ(wc != towlower(wc), iswupper(wc)); });
  }
}
