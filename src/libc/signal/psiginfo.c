// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <signal.h>

void psiginfo(const siginfo_t *pinfo, const char *message) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MESSAGES_MASK);
  psignal_l(pinfo->si_signo, message, locale);
}
