// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <string.h>

int strerror_r(int errnum, char *strerrbuf, size_t buflen) {
  // Fetch strings from the en_US locale directly.
  const struct lc_messages *messages = &__messages_en_us;
  size_t idx = errnum;
  if (idx < __arraycount(messages->strerror) &&
      messages->strerror[idx] != NULL) {
    strlcpy(strerrbuf, messages->strerror[idx], buflen);
  } else {
    strlcpy(strerrbuf, messages->unknown_error, buflen);
  }
  return 0;
}
