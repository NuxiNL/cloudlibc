// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <signal.h>

void psiginfo_l(const siginfo_t *pinfo, const char *message, locale_t locale) {
  psignal_l(pinfo->si_signo, message, locale);
}
