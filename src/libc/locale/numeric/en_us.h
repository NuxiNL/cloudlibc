// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

static const struct lc_numeric numeric_en_us = {
    .decimal_point = L".",
    .thousands_sep = L",",
    .grouping = (const signed char *)"\x03",
};
