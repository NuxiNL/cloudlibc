// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
