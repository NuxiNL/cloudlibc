// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

const struct lc_numeric __numeric_posix = {
    .decimal_point = L".",
    .grouping = "\x7f",  // CHAR_MAX.
};
