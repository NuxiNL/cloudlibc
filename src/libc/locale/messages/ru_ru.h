// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <errno.h>
#include <netdb.h>
#include <regex.h>
#include <signal.h>

static const struct lc_messages messages_ru_ru = {
    .yesexpr = "^[дДyY]", .noexpr = "^[нНnN]",

    // TODO(edje): Add error strings.
};
