// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>

void perror_l(const char *s, locale_t locale) {
  const struct lc_messages *messages = locale->messages;
  size_t idx = errno;
  char errstr[NL_TEXTMAX];
  if (idx < __arraycount(messages->strerror) &&
      messages->strerror[idx] != NULL) {
    __locale_translate_string(locale, errstr, messages->strerror[idx],
                              sizeof(errstr));
  } else {
    __locale_translate_string(locale, errstr, messages->unknown_error,
                              sizeof(errstr));
  }

  if (s == NULL || *s == '\0') {
    fprintf_l(stderr, locale, "%s\n", errstr);
  } else {
    fprintf_l(stderr, locale, "%s: %s\n", s, errstr);
  }
}
