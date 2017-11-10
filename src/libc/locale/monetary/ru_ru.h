// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_monetary monetary_ru_ru = {
    .mon_decimal_point = L",",
    .mon_thousands_sep = L" ",
    .mon_grouping = (const signed char *)"\x03",
    .negative_sign = L"-",
    .currency_symbol = L"руб.",
    .frac_digits = 2,
    .p_cs_precedes = 0,
    .p_sep_by_space = 1,
    .p_sign_posn = 1,
    .n_cs_precedes = 0,
    .n_sep_by_space = 1,
    .n_sign_posn = 1,
    .int_curr_symbol = "RUB \0",
    .int_frac_digits = 2,
    .int_p_cs_precedes = 0,
    .int_p_sep_by_space = 1,
    .int_p_sign_posn = 1,
    .int_n_cs_precedes = 0,
    .int_n_sep_by_space = 1,
    .int_n_sign_posn = 1,
};
