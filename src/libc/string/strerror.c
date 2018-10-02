// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strerror(int errnum) {
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

__weak_reference(__cloudlibc_strerror, strerror);
