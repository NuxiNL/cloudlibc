// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <locale.h>
#include <testing.h>

TEST(localeconv, posix) {
  // Values that localeconv() should return for the POSIX locale.
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
  ASSERT_EQ(lconv, localeconv_l(LC_POSIX_LOCALE));
}
