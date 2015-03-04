// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

void psignal_l(int signum, const char *message, locale_t locale) {
  // TODO(edje): Don't call strsignal() here.
  const char *sigstr = strsignal_l(signum, locale);
  if (message == NULL || *message == '\0') {
    fprintf_l(stderr, locale, "%s\n", sigstr);
  } else {
    fprintf_l(stderr, locale, "%s: %s\n", message, sigstr);
  }
}
