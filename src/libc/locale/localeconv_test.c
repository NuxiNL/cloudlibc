// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <locale.h>
#include <testing.h>

TEST(localeconv, c) {
  // Values that localeconv() should return for the C locale.
  struct lconv *lconv = localeconv();

  ASSERT_STREQ(".", lconv->decimal_point);
  ASSERT_STREQ("", lconv->thousands_sep);
  ASSERT_EQ(CHAR_MAX, lconv->grouping[0]);
  ASSERT_EQ('\0', lconv->grouping[1]);
  ASSERT_STREQ("", lconv->mon_decimal_point);
  ASSERT_STREQ("", lconv->mon_thousands_sep);
  ASSERT_EQ(CHAR_MAX, lconv->mon_grouping[0]);
  ASSERT_EQ(0, lconv->mon_grouping[1]);
  ASSERT_STREQ("", lconv->positive_sign);
  ASSERT_STREQ("", lconv->negative_sign);
  ASSERT_STREQ("", lconv->currency_symbol);
  ASSERT_EQ(CHAR_MAX, lconv->frac_digits);
  ASSERT_EQ(CHAR_MAX, lconv->p_cs_precedes);
  ASSERT_EQ(CHAR_MAX, lconv->p_sep_by_space);
  ASSERT_EQ(CHAR_MAX, lconv->p_sign_posn);
  ASSERT_EQ(CHAR_MAX, lconv->n_cs_precedes);
  ASSERT_EQ(CHAR_MAX, lconv->n_sep_by_space);
  ASSERT_EQ(CHAR_MAX, lconv->n_sign_posn);
  ASSERT_STREQ("", lconv->int_curr_symbol);
  ASSERT_EQ(CHAR_MAX, lconv->int_frac_digits);
  ASSERT_EQ(CHAR_MAX, lconv->int_p_cs_precedes);
  ASSERT_EQ(CHAR_MAX, lconv->int_p_sep_by_space);
  ASSERT_EQ(CHAR_MAX, lconv->int_p_sign_posn);
  ASSERT_EQ(CHAR_MAX, lconv->int_n_cs_precedes);
  ASSERT_EQ(CHAR_MAX, lconv->int_n_sep_by_space);
  ASSERT_EQ(CHAR_MAX, lconv->int_n_sign_posn);

  // Object should remain cached.
  ASSERT_EQ(lconv, localeconv());
  ASSERT_EQ(lconv, localeconv_l(LC_C_LOCALE));
}

TEST(localeconv, nl_nl) {
  // Values that localeconv() should return for the Dutch locale.
  locale_t locale = newlocale(LC_ALL_MASK, "nl_NL.UTF-8@Europe/Amsterdam", 0);
  struct lconv *lconv = localeconv_l(locale);

  ASSERT_STREQ("€", lconv->currency_symbol);
  ASSERT_STREQ(",", lconv->decimal_point);
  ASSERT_EQ(2, lconv->frac_digits);
  ASSERT_EQ(CHAR_MAX, lconv->grouping[0]);
  ASSERT_EQ('\0', lconv->grouping[1]);
  ASSERT_STREQ("EUR ", lconv->int_curr_symbol);
  ASSERT_EQ(2, lconv->int_frac_digits);
  ASSERT_EQ(1, lconv->int_n_cs_precedes);
  ASSERT_EQ(1, lconv->int_n_sep_by_space);
  ASSERT_EQ(2, lconv->int_n_sign_posn);
  ASSERT_EQ(1, lconv->int_p_cs_precedes);
  ASSERT_EQ(1, lconv->int_p_sep_by_space);
  ASSERT_EQ(1, lconv->int_p_sign_posn);
  ASSERT_STREQ(",", lconv->mon_decimal_point);
  ASSERT_STREQ("\x03\x03", lconv->mon_grouping);
  ASSERT_STREQ(" ", lconv->mon_thousands_sep);
  ASSERT_STREQ("-", lconv->negative_sign);
  ASSERT_EQ(1, lconv->n_cs_precedes);
  ASSERT_EQ(1, lconv->n_sep_by_space);
  ASSERT_EQ(2, lconv->n_sign_posn);
  ASSERT_STREQ("", lconv->positive_sign);
  ASSERT_EQ(1, lconv->p_cs_precedes);
  ASSERT_EQ(1, lconv->p_sep_by_space);
  ASSERT_EQ(1, lconv->p_sign_posn);
  ASSERT_STREQ(" ", lconv->thousands_sep);

  freelocale(locale);
}
