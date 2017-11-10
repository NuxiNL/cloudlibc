// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

static const struct lc_numeric numeric_en_us = {
    .decimal_point = L".",
    .thousands_sep = L",",
    .grouping = (const signed char *)"\x03",
};
