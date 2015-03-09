// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <testing.h>

TEST(newlocale, zero) {
  // Empty mask should yield the standard locale.
  locale_t locale = newlocale(0, "Unknown", 0);
  ASSERT_EQ(LC_GLOBAL_LOCALE, locale);
  freelocale(locale);
}

TEST(newlocale, unknown) {
  // Empty mask should yield the standard locale.
  locale_t locale = newlocale(LC_ALL_MASK, "Unknown", 0);
  ASSERT_EQ(0, locale);
  ASSERT_EQ(ENOENT, errno);
}

TEST(newlocale, valid_timezone) {
  // If we're only interested in the timezone, it doesn't matter if the
  // language and character set are valid.
  locale_t locale =
      newlocale(LC_TIMEZONE_MASK, "zz_ZZ.ZZZZ@Europe/Amsterdam", 0);
  ASSERT_NE(LC_GLOBAL_LOCALE, locale);
  freelocale(locale);
}

TEST(newlocale, deduplication) {
  // en_US uses the same date/time formatting as the C locale. There's
  // no need to allocate a new locale object.
  locale_t locale = newlocale(LC_TIME_MASK, "en_US", 0);
  ASSERT_EQ(LC_GLOBAL_LOCALE, locale);
  freelocale(locale);
}

TEST(newlocale, charset_only) {
  // It should be possible to provide just a character set.
  locale_t locale = newlocale(LC_CTYPE_MASK, ".UTF-8", 0);
  ASSERT_NE(LC_GLOBAL_LOCALE, locale);
  freelocale(locale);
}
