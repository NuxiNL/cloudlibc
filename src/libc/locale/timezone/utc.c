// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

// Repeat the definition of the default timezone (UTC). This way we
// don't need to pull in the entire timezone database for the trivial
// cases.
static const struct lc_timezone_era timezone_era_utc = {
    .abbreviation_std = "UTC",
};
const struct lc_timezone __timezone_utc = {&timezone_era_utc, 1};
