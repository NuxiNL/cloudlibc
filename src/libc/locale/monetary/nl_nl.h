// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <limits.h>

// TODO(edje): Make more accurate.
static const struct lc_monetary monetary_nl_nl = {
    .mon_decimal_point = L",",
    .mon_thousands_sep = L" ",
    .mon_grouping = "\x7f",  // CHAR_MAX.
    .currency_symbol = L"€",
    .frac_digits = CHAR_MAX,
    .p_cs_precedes = 1,
    .p_sep_by_space = CHAR_MAX,
    .p_sign_posn = CHAR_MAX,
    .n_cs_precedes = 1,
    .n_sep_by_space = CHAR_MAX,
    .n_sign_posn = CHAR_MAX,
    .int_frac_digits = CHAR_MAX,
    .int_p_cs_precedes = CHAR_MAX,
    .int_p_sep_by_space = CHAR_MAX,
    .int_p_sign_posn = CHAR_MAX,
    .int_n_cs_precedes = CHAR_MAX,
    .int_n_sep_by_space = CHAR_MAX,
    .int_n_sign_posn = CHAR_MAX,
};
