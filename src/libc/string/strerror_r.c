// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <string.h>

int strerror_r(int errnum, char *strerrbuf, size_t buflen) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK | LC_MESSAGES_MASK);
  const struct lc_messages *messages = locale->messages;
  size_t idx = errnum;
  if (idx < __arraycount(messages->strerror) &&
      messages->strerror[idx] != NULL) {
    __locale_translate_string(locale, strerrbuf,
                              locale->messages->strerror[idx], buflen);
  } else {
    __locale_translate_string(locale, strerrbuf,
                              locale->messages->unknown_error, buflen);
  }
  return 0;
}
