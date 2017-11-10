// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <errno.h>
#include <netdb.h>
#include <regex.h>
#include <signal.h>

static const struct lc_messages messages_ru_ru = {
    .yesexpr = "^[дДyY]", .noexpr = "^[нНnN]",

    // TODO(ed): Add error strings.
};
