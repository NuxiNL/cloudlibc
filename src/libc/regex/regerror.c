// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <regex.h>
#include <string.h>

size_t regerror(int errcode, const regex_t *restrict preg,
                char *restrict errbuf, size_t errbuf_size) {
  // Fetch strings from the en_US locale directly.
  const struct lc_messages *messages = &__messages_en_us;
  size_t idx = errcode;
  if (idx < __arraycount(messages->regerror) &&
      messages->regerror[idx] != NULL) {
    return strlcpy(errbuf, messages->regerror[idx], errbuf_size) + 1;
  } else {
    return strlcpy(errbuf, messages->unknown_error, errbuf_size) + 1;
  }
}
