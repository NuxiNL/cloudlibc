// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/clock.h>

#include <cloudabi_types.h>
#include <time.h>

const struct __clockid _CLOCK_PROCESS_CPUTIME_ID = {
    .id = CLOUDABI_CLOCK_PROCESS_CPUTIME_ID,
};
