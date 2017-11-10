// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
