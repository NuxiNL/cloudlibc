// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <limits.h>
#include <signal.h>
#include <stdio.h>

void psignal_l(int signum, const char *message, locale_t locale) {
  const struct lc_messages *messages = locale->messages;
  size_t idx = signum;
  if (idx >= __arraycount(messages->strsignal) ||
      messages->strsignal[idx] == NULL)
    idx = 0;
  char sigstr[NL_TEXTMAX];
  __locale_translate_string(locale, sigstr, messages->strsignal[idx],
                            sizeof(sigstr));

  if (message == NULL || *message == '\0') {
    fprintf_l(stderr, locale, "%s\n", sigstr);
  } else {
    fprintf_l(stderr, locale, "%s: %s\n", message, sigstr);
  }
}
