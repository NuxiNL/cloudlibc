// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <errno.h>
#include <netdb.h>
#include <regex.h>
#include <signal.h>

static const struct lc_messages messages_nl_nl = {
    .yesexpr = "^[jJyY]",
    .noexpr = "^[nN]",

    // TODO(ed): Add error strings.

    .unknown_error = "Onbekende fout",
};
