// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

// localedump dumps the values of nl_langinfo() and localeconv() for the
// current locale. This can be used to validate whether the locale data
// is in sync with other operating systems.

#include <langinfo.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>

int main() {
  setlocale(LC_ALL, "");
#define ENTRY(name)                                                     \
  printf("  ASSERT_STREQ(\"%s\", nl_langinfo_l(" #name ", locale));\n", \
         nl_langinfo(name))
  ENTRY(CODESET);
  ENTRY(D_T_FMT);
  ENTRY(D_FMT);
  ENTRY(T_FMT);
  ENTRY(T_FMT_AMPM);
  ENTRY(AM_STR);
  ENTRY(PM_STR);
  ENTRY(DAY_1);
  ENTRY(DAY_2);
  ENTRY(DAY_3);
  ENTRY(DAY_4);
  ENTRY(DAY_5);
  ENTRY(DAY_6);
  ENTRY(DAY_7);
  ENTRY(ABDAY_1);
  ENTRY(ABDAY_2);
  ENTRY(ABDAY_3);
  ENTRY(ABDAY_4);
  ENTRY(ABDAY_5);
  ENTRY(ABDAY_6);
  ENTRY(ABDAY_7);
  ENTRY(MON_1);
  ENTRY(MON_2);
  ENTRY(MON_3);
  ENTRY(MON_4);
  ENTRY(MON_5);
  ENTRY(MON_6);
  ENTRY(MON_7);
  ENTRY(MON_8);
  ENTRY(MON_9);
  ENTRY(MON_10);
  ENTRY(MON_11);
  ENTRY(MON_12);
  ENTRY(ABMON_1);
  ENTRY(ABMON_2);
  ENTRY(ABMON_3);
  ENTRY(ABMON_4);
  ENTRY(ABMON_5);
  ENTRY(ABMON_6);
  ENTRY(ABMON_7);
  ENTRY(ABMON_8);
  ENTRY(ABMON_9);
  ENTRY(ABMON_10);
  ENTRY(ABMON_11);
  ENTRY(ABMON_12);
  ENTRY(ERA);
  ENTRY(ERA_D_FMT);
  ENTRY(ERA_D_T_FMT);
  ENTRY(ERA_T_FMT);
  ENTRY(ALT_DIGITS);
  ENTRY(RADIXCHAR);
  ENTRY(THOUSEP);
  ENTRY(YESEXPR);
  ENTRY(NOEXPR);
  ENTRY(CRNCYSTR);
#undef ENTRY

  struct lconv *lconv = localeconv();
  printf("\n");
#define ENTRY_CHAR(field)                                  \
  if (lconv->field == CHAR_MAX)                            \
    printf("  ASSERT_EQ(CHAR_MAX, lconv->" #field ");\n"); \
  else                                                     \
    printf("  ASSERT_EQ(%hhu, lconv->" #field ");\n", lconv->field);
#define ENTRY_STRING(field) \
  printf("  ASSERT_STREQ(\"%s\", lconv->" #field ");\n", lconv->field);
  ENTRY_STRING(currency_symbol);
  ENTRY_STRING(decimal_point);
  ENTRY_CHAR(frac_digits);
  ENTRY_STRING(grouping);
  ENTRY_STRING(int_curr_symbol);
  ENTRY_CHAR(int_frac_digits);
  ENTRY_CHAR(int_n_cs_precedes);
  ENTRY_CHAR(int_n_sep_by_space);
  ENTRY_CHAR(int_n_sign_posn);
  ENTRY_CHAR(int_p_cs_precedes);
  ENTRY_CHAR(int_p_sep_by_space);
  ENTRY_CHAR(int_p_sign_posn);
  ENTRY_STRING(mon_decimal_point);
  ENTRY_STRING(mon_grouping);
  ENTRY_STRING(mon_thousands_sep);
  ENTRY_STRING(negative_sign);
  ENTRY_CHAR(n_cs_precedes);
  ENTRY_CHAR(n_sep_by_space);
  ENTRY_CHAR(n_sign_posn);
  ENTRY_STRING(positive_sign);
  ENTRY_CHAR(p_cs_precedes);
  ENTRY_CHAR(p_sep_by_space);
  ENTRY_CHAR(p_sign_posn);
  ENTRY_STRING(thousands_sep);
#undef ENTRY_CHAR
#undef ENTRY_STRING
}
