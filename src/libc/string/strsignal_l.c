// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
