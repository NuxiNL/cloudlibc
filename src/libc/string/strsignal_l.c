// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <string.h>

char *strsignal_l(int signum, locale_t locale) {
  const struct lc_messages *messages = locale->messages;
  size_t idx = signum;
  if (idx >= __arraycount(messages->strsignal) ||
      messages->strsignal[idx] == NULL)
    idx = 0;
  return (char *)COMPILE_STRING(locale, messages, strsignal[idx]);
}
