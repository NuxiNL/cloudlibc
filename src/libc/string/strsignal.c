// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <string.h>

char *strsignal(int signum) {
  // Fetch strings from the en_US locale directly.
  const struct lc_messages *messages = &__messages_en_us;
  size_t idx = signum;
  if (idx >= __arraycount(messages->strsignal) ||
      messages->strsignal[idx] == NULL)
    idx = 0;
  return (char *)messages->strsignal[idx];
}
