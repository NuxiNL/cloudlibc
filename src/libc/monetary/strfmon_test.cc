// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <float.h>
#include <locale.h>
#include <monetary.h>

#include "gtest/gtest.h"

#define TEST_STRFMON(out, ...)                                           \
  do {                                                                   \
    char buf[sizeof(out)];                                               \
    ASSERT_EQ(sizeof(buf) - 1,                                           \
              strfmon_l(buf, sizeof(buf), locale, __VA_ARGS__));         \
    ASSERT_STREQ(out, buf);                                              \
    ASSERT_EQ(-1, strfmon_l(buf, sizeof(buf) - 1, locale, __VA_ARGS__)); \
    ASSERT_EQ(E2BIG, errno);                                             \
  } while (0)

TEST(strfmon, en_us) {
  locale_t locale = newlocale(LC_MONETARY_MASK, "en_US", 0);
  ASSERT_NE((locale_t)0, locale);

  TEST_STRFMON("$0.00", "%n", 0.0);
  TEST_STRFMON("$0.01", "%n", 0.01);
  TEST_STRFMON("-$0.42", "%n", -0.42);
  TEST_STRFMON("$123.45", "%n", 123.45);
  TEST_STRFMON("-$123.45", "%n", -123.45);
  TEST_STRFMON("$3,456.78", "%n", 3456.781);
#if DBL_MANT_DIG == 53
  // In case the number of digits is larger than the precision of the
  // floating point type, this implementation will just print trailing
  // zeroes.
  TEST_STRFMON(
      "$179,769,313,486,231,570,000,000,000,000,000,000,000,000,000,000,000,"
      "000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,"
      "000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,"
      "000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,"
      "000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,"
      "000,000,000,000,000,000,000,000,000,000,000,000,000,000.00",
      "%n", DBL_MAX);
#else
#error "Unsupported test"
#endif

  TEST_STRFMON("% Value: $0.00!", "%% Value: %n!", 0.0);
  TEST_STRFMON("% Value: $0.01!", "%% Value: %n!", 0.01);
  TEST_STRFMON("% Value: -$0.42!", "%% Value: %n!", -0.42);
  TEST_STRFMON("% Value: $123.45!", "%% Value: %n!", 123.45);
  TEST_STRFMON("% Value: -$123.45!", "%% Value: %n!", -123.45);
  TEST_STRFMON("% Value: $3,456.78!", "%% Value: %n!", 3456.781);

  TEST_STRFMON("      $0.00", "%11n", 0.0);
  TEST_STRFMON("      $0.00", "%11n", 0.004);
  TEST_STRFMON("      $0.01", "%11n", 0.005);
  TEST_STRFMON("      $0.01", "%11n", 0.01);
  TEST_STRFMON("     -$0.42", "%11n", -0.42);
  TEST_STRFMON("    $123.45", "%11n", 123.45);
  TEST_STRFMON("   -$123.45", "%11n", -123.45);
  TEST_STRFMON("  $3,456.78", "%11n", 3456.781);

  TEST_STRFMON(" $     0.00", "%#5n", 0.0);
  TEST_STRFMON(" $     0.01", "%#5n", 0.01);
  TEST_STRFMON("-$     0.42", "%#5n", -0.42);
  TEST_STRFMON(" $   123.45", "%#5n", 123.45);
  TEST_STRFMON("-$   123.45", "%#5n", -123.45);
  TEST_STRFMON(" $ 3,456.78", "%#5n", 3456.781);

  TEST_STRFMON(" $*****0.00", "%=*#5n", 0.0);
  TEST_STRFMON(" $*****0.01", "%=*#5n", 0.01);
  TEST_STRFMON("-$*****0.42", "%=*#5n", -0.42);
  TEST_STRFMON(" $***123.45", "%=*#5n", 123.45);
  TEST_STRFMON("-$***123.45", "%=*#5n", -123.45);
  TEST_STRFMON(" $*3,456.78", "%=*#5n", 3456.781);

  TEST_STRFMON(" $000000.00", "%=0#5n", 0.0);
  TEST_STRFMON(" $000000.01", "%=0#5n", 0.01);
  TEST_STRFMON("-$000000.42", "%=0#5n", -0.42);
  TEST_STRFMON(" $000123.45", "%=0#5n", 123.45);
  TEST_STRFMON("-$000123.45", "%=0#5n", -123.45);
  TEST_STRFMON(" $03,456.78", "%=0#5n", 3456.781);

  TEST_STRFMON(" $    0.00", "%^#5n", 0.0);
  TEST_STRFMON(" $    0.01", "%^#5n", 0.01);
  TEST_STRFMON("-$    0.42", "%^#5n", -0.42);
  TEST_STRFMON(" $  123.45", "%^#5n", 123.45);
  TEST_STRFMON("-$  123.45", "%^#5n", -123.45);
  TEST_STRFMON(" $ 3456.78", "%^#5n", 3456.781);

  TEST_STRFMON(" $    0", "%^#5.0n", 0.0);
  TEST_STRFMON(" $    0", "%^#5.0n", 0.01);
  TEST_STRFMON("-$    0", "%^#5.0n", -0.42);
  TEST_STRFMON(" $  123", "%^#5.0n", 123.45);
  TEST_STRFMON("-$  123", "%^#5.0n", -123.45);
  TEST_STRFMON(" $ 3457", "%^#5.0n", 3456.781);

  TEST_STRFMON(" $    0.0000", "%^#5.4n", 0.0);
  TEST_STRFMON(" $    0.0100", "%^#5.4n", 0.01);
  TEST_STRFMON("-$    0.4200", "%^#5.4n", -0.42);
  TEST_STRFMON(" $  123.4500", "%^#5.4n", 123.45);
  TEST_STRFMON("-$  123.4500", "%^#5.4n", -123.45);
  TEST_STRFMON(" $ 3456.7810", "%^#5.4n", 3456.781);

  TEST_STRFMON(" $     0.00 ", "%(#5n", 0.0);
  TEST_STRFMON(" $     0.01 ", "%(#5n", 0.01);
  TEST_STRFMON("($     0.42)", "%(#5n", -0.42);
  TEST_STRFMON(" $   123.45 ", "%(#5n", 123.45);
  TEST_STRFMON("($   123.45)", "%(#5n", -123.45);
  TEST_STRFMON(" $ 3,456.78 ", "%(#5n", 3456.781);

  TEST_STRFMON("      0.00 ", "%!(#5n", 0.0);
  TEST_STRFMON("      0.01 ", "%!(#5n", 0.01);
  TEST_STRFMON("(     0.42)", "%!(#5n", -0.42);
  TEST_STRFMON("    123.45 ", "%!(#5n", 123.45);
  TEST_STRFMON("(   123.45)", "%!(#5n", -123.45);
  TEST_STRFMON("  3,456.78 ", "%!(#5n", 3456.781);

  TEST_STRFMON(" $     0.0000 ", "%-14#5.4n", 0.0);
  TEST_STRFMON(" $     0.0100 ", "%-14#5.4n", 0.01);
  TEST_STRFMON("-$     0.4200 ", "%-14#5.4n", -0.42);
  TEST_STRFMON(" $   123.4500 ", "%-14#5.4n", 123.45);
  TEST_STRFMON("-$   123.4500 ", "%-14#5.4n", -123.45);
  TEST_STRFMON(" $ 3,456.7810 ", "%-14#5.4n", 3456.781);

  TEST_STRFMON("  $     0.0000", "%14#5.4n", 0.0);
  TEST_STRFMON("  $     0.0100", "%14#5.4n", 0.01);
  TEST_STRFMON(" -$     0.4200", "%14#5.4n", -0.42);
  TEST_STRFMON("  $   123.4500", "%14#5.4n", 123.45);
  TEST_STRFMON(" -$   123.4500", "%14#5.4n", -123.45);
  TEST_STRFMON("  $ 3,456.7810", "%14#5.4n", 3456.781);

  TEST_STRFMON("USD 0.00", "%i", 0.0);
  TEST_STRFMON("USD 0.01", "%i", 0.01);
  TEST_STRFMON("-USD 0.42", "%i", -0.42);
  TEST_STRFMON("USD 123.45", "%i", 123.45);
  TEST_STRFMON("-USD 123.45", "%i", -123.45);
  TEST_STRFMON("USD 3,456.78", "%i", 3456.781);

  freelocale(locale);
}

TEST(strfmon, nl_nl) {
  locale_t locale =
      newlocale(LC_CTYPE_MASK | LC_MONETARY_MASK, "nl_NL.UTF-8", 0);
  ASSERT_NE((locale_t)0, locale);

  TEST_STRFMON("€ 0,00", "%n", 0.0);
  TEST_STRFMON("€ 0,01", "%n", 0.01);
  TEST_STRFMON("€ 0,42-", "%n", -0.42);
  TEST_STRFMON("€ 123,45", "%n", 123.45);
  TEST_STRFMON("€ 123,45-", "%n", -123.45);
  TEST_STRFMON("€ 3 456,78", "%n", 3456.781);

  TEST_STRFMON("      € 0,00", "%14n", 0.0);
  TEST_STRFMON("      € 0,01", "%14n", 0.01);
  TEST_STRFMON("     € 0,42-", "%14n", -0.42);
  TEST_STRFMON("    € 123,45", "%14n", 123.45);
  TEST_STRFMON("   € 123,45-", "%14n", -123.45);
  TEST_STRFMON("  € 3 456,78", "%14n", 3456.781);

  TEST_STRFMON("€   0,00 ", "%#3n", 0.0);
  TEST_STRFMON("€   0,01 ", "%#3n", 0.01);
  TEST_STRFMON("€   0,42-", "%#3n", -0.42);
  TEST_STRFMON("€ 123,45 ", "%#3n", 123.45);
  TEST_STRFMON("€ 123,45-", "%#3n", -123.45);
  TEST_STRFMON("€ 3 456,78 ", "%#3n", 3456.781);

  TEST_STRFMON("€ 0,00", "%(n", 0.0);
  TEST_STRFMON("€ 0,01", "%(n", 0.01);
  TEST_STRFMON("(€ 0,42)", "%(n", -0.42);
  TEST_STRFMON("€ 123,45", "%(n", 123.45);
  TEST_STRFMON("(€ 123,45)", "%(n", -123.45);
  TEST_STRFMON("€ 3 456,78", "%(n", 3456.781);

  TEST_STRFMON("EUR 0,00", "%i", 0.0);
  TEST_STRFMON("EUR 0,01", "%i", 0.01);
  TEST_STRFMON("EUR 0,42-", "%i", -0.42);
  TEST_STRFMON("EUR 123,45", "%i", 123.45);
  TEST_STRFMON("EUR 123,45-", "%i", -123.45);
  TEST_STRFMON("EUR 3 456,78", "%i", 3456.781);

  freelocale(locale);
}
