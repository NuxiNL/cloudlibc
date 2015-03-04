// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <testing.h>

TEST(duplocale, example) {
  locale_t locale = duplocale(LC_GLOBAL_LOCALE);
  ASSERT_EQ(LC_GLOBAL_LOCALE, locale);
  freelocale(locale);
}
