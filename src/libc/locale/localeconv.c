// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <locale.h>

struct lconv *localeconv(void) {
  // Return a fixed copy with the C locale values.
  static const struct lconv lconv = {
      .decimal_point = (char *)".",
      .thousands_sep = (char *)"",
      .grouping = (char *)"",
      .mon_decimal_point = (char *)"",
      .mon_thousands_sep = (char *)"",
      .mon_grouping = (char *)"",
      .positive_sign = (char *)"",
      .negative_sign = (char *)"",
      .currency_symbol = (char *)"",
      .frac_digits = CHAR_MAX,
      .p_cs_precedes = CHAR_MAX,
      .p_sep_by_space = CHAR_MAX,
      .p_sign_posn = CHAR_MAX,
      .n_cs_precedes = CHAR_MAX,
      .n_sep_by_space = CHAR_MAX,
      .n_sign_posn = CHAR_MAX,
      .int_curr_symbol = (char *)"",
      .int_frac_digits = CHAR_MAX,
      .int_p_cs_precedes = CHAR_MAX,
      .int_p_sep_by_space = CHAR_MAX,
      .int_p_sign_posn = CHAR_MAX,
      .int_n_cs_precedes = CHAR_MAX,
      .int_n_sep_by_space = CHAR_MAX,
      .int_n_sign_posn = CHAR_MAX,
  };
  return (struct lconv *)&lconv;
}
