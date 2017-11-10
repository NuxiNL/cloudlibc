// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <string.h>

char *strerror_l(int errnum, locale_t locale) {
  const struct lc_messages *messages = locale->messages;
  size_t idx = errnum;
  if (idx < __arraycount(messages->strerror) &&
      messages->strerror[idx] != NULL) {
    return (char *)COMPILE_STRING(locale, messages, strerror[idx]);
  } else {
    return (char *)COMPILE_STRING(locale, messages, unknown_error);
  }
}
