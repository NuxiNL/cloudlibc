// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <regex.h>

size_t regerror_l(int errcode, const regex_t *restrict preg,
                  char *restrict errbuf, size_t errbuf_size, locale_t locale) {
  const struct lc_messages *messages = locale->messages;
  size_t idx = errcode;
  if (idx < __arraycount(messages->regerror) &&
      messages->regerror[idx] != NULL) {
    return __locale_translate_string(locale, errbuf, messages->regerror[idx],
                                     errbuf_size);
  } else {
    return __locale_translate_string(locale, errbuf, messages->unknown_error,
                                     errbuf_size);
  }
}
