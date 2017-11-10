// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <netdb.h>

const char *gai_strerror_l(int ecode, locale_t locale) {
  const struct lc_messages *messages = locale->messages;
  size_t idx = ecode;
  if (idx < __arraycount(messages->gai_strerror) &&
      messages->gai_strerror[idx] != NULL) {
    return (char *)COMPILE_STRING(locale, messages, gai_strerror[idx]);
  } else {
    return (char *)COMPILE_STRING(locale, messages, unknown_error);
  }
}
