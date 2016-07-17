// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <string.h>

char *strerror(int errnum) {
  // Fetch strings from the en_US locale directly.
  const struct lc_messages *messages = &__messages_en_us;
  size_t idx = errnum;
  if (idx < __arraycount(messages->strerror) &&
      messages->strerror[idx] != NULL) {
    return (char *)messages->strerror[idx];
  } else {
    return (char *)messages->unknown_error;
  }
}
