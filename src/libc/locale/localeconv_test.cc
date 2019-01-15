// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <locale.h>

#include "gtest/gtest.h"

TEST(localeconv, c_standard) {
  // Values that localeconv() should return for the C locale.
  struct lconv *lconv = localeconv();

  ASSERT_STREQ(".", lconv->decimal_point);
  ASSERT_STREQ("", lconv->thousands_sep);
  ASSERT_STREQ("", lconv->grouping);
  ASSERT_STREQ("", lconv->mon_decimal_point);
  ASSERT_STREQ("", lconv->mon_thousands_sep);
  ASSERT_EQ("", lconv->mon_grouping);
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
}

TEST(localeconv, c_object) {
  // Values that localeconv() should return for the C locale.
  struct lconv *lconv = localeconv_l(LC_C_LOCALE);

  ASSERT_STREQ(".", lconv->decimal_point);
  ASSERT_STREQ("", lconv->thousands_sep);
  ASSERT_STREQ("", lconv->grouping);
  ASSERT_STREQ("", lconv->mon_decimal_point);
  ASSERT_STREQ("", lconv->mon_thousands_sep);
  ASSERT_EQ("", lconv->mon_grouping);
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
  ASSERT_EQ(lconv, localeconv_l(LC_C_LOCALE));
}

TEST(localeconv, en_us) {
  // Values that localeconv() should return for the US English locale.
  locale_t locale = newlocale(LC_ALL_MASK, "en_US.UTF-8@America/New_York", 0);
  struct lconv *lconv = localeconv_l(locale);

  ASSERT_STREQ("$", lconv->currency_symbol);
  ASSERT_STREQ(".", lconv->decimal_point);
  ASSERT_EQ(2, lconv->frac_digits);
  ASSERT_STREQ("\x03", lconv->grouping);
  ASSERT_STREQ("USD ", lconv->int_curr_symbol);
  ASSERT_EQ(2, lconv->int_frac_digits);
  ASSERT_EQ(1, lconv->int_n_cs_precedes);
  ASSERT_EQ(1, lconv->int_n_sep_by_space);
  ASSERT_EQ(1, lconv->int_n_sign_posn);
  ASSERT_EQ(1, lconv->int_p_cs_precedes);
  ASSERT_EQ(1, lconv->int_p_sep_by_space);
  ASSERT_EQ(1, lconv->int_p_sign_posn);
  ASSERT_STREQ(".", lconv->mon_decimal_point);
  ASSERT_STREQ("\x03", lconv->mon_grouping);
  ASSERT_STREQ(",", lconv->mon_thousands_sep);
  ASSERT_STREQ("-", lconv->negative_sign);
  ASSERT_EQ(1, lconv->n_cs_precedes);
  ASSERT_EQ(0, lconv->n_sep_by_space);
  ASSERT_EQ(1, lconv->n_sign_posn);
  ASSERT_STREQ("", lconv->positive_sign);
  ASSERT_EQ(1, lconv->p_cs_precedes);
  ASSERT_EQ(0, lconv->p_sep_by_space);
  ASSERT_EQ(1, lconv->p_sign_posn);
  ASSERT_STREQ(",", lconv->thousands_sep);

  // Object should remain cached.
  ASSERT_EQ(lconv, localeconv_l(locale));
  freelocale(locale);
}

TEST(localeconv, nl_nl) {
  // Values that localeconv() should return for the Dutch locale.
  locale_t locale = newlocale(LC_ALL_MASK, "nl_NL.UTF-8@Europe/Amsterdam", 0);
  struct lconv *lconv = localeconv_l(locale);

  ASSERT_STREQ("€", lconv->currency_symbol);
  ASSERT_STREQ(",", lconv->decimal_point);
  ASSERT_EQ(2, lconv->frac_digits);
  ASSERT_STREQ("", lconv->grouping);
  ASSERT_STREQ("EUR ", lconv->int_curr_symbol);
  ASSERT_EQ(2, lconv->int_frac_digits);
  ASSERT_EQ(1, lconv->int_n_cs_precedes);
  ASSERT_EQ(1, lconv->int_n_sep_by_space);
  ASSERT_EQ(2, lconv->int_n_sign_posn);
  ASSERT_EQ(1, lconv->int_p_cs_precedes);
  ASSERT_EQ(1, lconv->int_p_sep_by_space);
  ASSERT_EQ(1, lconv->int_p_sign_posn);
  ASSERT_STREQ(",", lconv->mon_decimal_point);
  ASSERT_STREQ("\x03", lconv->mon_grouping);
  ASSERT_STREQ(" ", lconv->mon_thousands_sep);
  ASSERT_STREQ("-", lconv->negative_sign);
  ASSERT_EQ(1, lconv->n_cs_precedes);
  ASSERT_EQ(1, lconv->n_sep_by_space);
  ASSERT_EQ(2, lconv->n_sign_posn);
  ASSERT_STREQ("", lconv->positive_sign);
  ASSERT_EQ(1, lconv->p_cs_precedes);
  ASSERT_EQ(1, lconv->p_sep_by_space);
  ASSERT_EQ(1, lconv->p_sign_posn);
  ASSERT_STREQ("", lconv->thousands_sep);

  // Object should remain cached.
  ASSERT_EQ(lconv, localeconv_l(locale));
  freelocale(locale);
}

TEST(localeconv, ru_ru) {
  // Values that localeconv() should return for the Russian locale.
  locale_t locale = newlocale(LC_ALL_MASK, "ru_RU.UTF-8@Europe/Moscow", 0);
  struct lconv *lconv = localeconv_l(locale);

  ASSERT_STREQ("руб.", lconv->currency_symbol);
  ASSERT_STREQ(",", lconv->decimal_point);
  ASSERT_EQ(2, lconv->frac_digits);
  ASSERT_STREQ("\x03", lconv->grouping);
  ASSERT_STREQ("RUB ", lconv->int_curr_symbol);
  ASSERT_EQ(2, lconv->int_frac_digits);
  ASSERT_EQ(0, lconv->int_n_cs_precedes);
  ASSERT_EQ(1, lconv->int_n_sep_by_space);
  ASSERT_EQ(1, lconv->int_n_sign_posn);
  ASSERT_EQ(0, lconv->int_p_cs_precedes);
  ASSERT_EQ(1, lconv->int_p_sep_by_space);
  ASSERT_EQ(1, lconv->int_p_sign_posn);
  ASSERT_STREQ(",", lconv->mon_decimal_point);
  ASSERT_STREQ("\x03", lconv->mon_grouping);
  ASSERT_STREQ(" ", lconv->mon_thousands_sep);
  ASSERT_STREQ("-", lconv->negative_sign);
  ASSERT_EQ(0, lconv->n_cs_precedes);
  ASSERT_EQ(1, lconv->n_sep_by_space);
  ASSERT_EQ(1, lconv->n_sign_posn);
  ASSERT_STREQ("", lconv->positive_sign);
  ASSERT_EQ(0, lconv->p_cs_precedes);
  ASSERT_EQ(1, lconv->p_sep_by_space);
  ASSERT_EQ(1, lconv->p_sign_posn);
  ASSERT_STREQ(" ", lconv->thousands_sep);

  // Object should remain cached.
  ASSERT_EQ(lconv, localeconv_l(locale));
  freelocale(locale);
}
