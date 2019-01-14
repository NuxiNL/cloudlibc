// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

#include "gtest/gtest.h"

TEST(iswlower, equality) {
  for (wint_t wc = 0; wc <= 0x10ffff; ++wc) {
    SCOPED_TRACE(wc);
    ASSERT_EQ(wc != towupper(wc), iswlower(wc));
  }
}
