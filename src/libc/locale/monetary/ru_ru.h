// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <limits.h>

static const struct lc_monetary monetary_ru_ru = {
    .mon_decimal_point = L",",
    .mon_thousands_sep = L" ",
    .mon_grouping = "\x03\x03",
    .negative_sign = L"-",
    .currency_symbol = L"руб.",
    .frac_digits = 2,
    .p_cs_precedes = 0,
    .p_sep_by_space = 1,
    .p_sign_posn = 1,
    .n_cs_precedes = 0,
    .n_sep_by_space = 1,
    .n_sign_posn = 1,
    .int_curr_symbol = L"RUB ",
    .int_frac_digits = 2,
    .int_p_cs_precedes = 0,
    .int_p_sep_by_space = 1,
    .int_p_sign_posn = 1,
    .int_n_cs_precedes = 0,
    .int_n_sep_by_space = 1,
    .int_n_sign_posn = 1,
};
