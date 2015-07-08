// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
