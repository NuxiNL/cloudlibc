// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>

#include "gtest/gtest.h"

TEST(duplocale, example) {
  locale_t locale = duplocale(LC_GLOBAL_LOCALE);
  ASSERT_EQ(LC_GLOBAL_LOCALE, locale);
  freelocale(locale);
}
