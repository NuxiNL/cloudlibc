// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <netdb.h>

const char *gai_strerror(int ecode) {
  // Fetch strings from the en_US locale directly.
  const struct lc_messages *messages = &__messages_en_us;
  size_t idx = ecode;
  if (idx < __arraycount(messages->gai_strerror) &&
      messages->gai_strerror[idx] != NULL) {
    return (char *)messages->gai_strerror[idx];
  } else {
    return (char *)messages->unknown_error;
  }
}
