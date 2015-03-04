// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

void perror_l(const char *s, locale_t locale) {
  // TODO(edje): Don't call strerror() here.
  const char *errstr = strerror_l(errno, locale);
  if (s == NULL || *s == '\0') {
    fprintf_l(stderr, locale, "%s\n", errstr);
  } else {
    fprintf_l(stderr, locale, "%s: %s\n", s, errstr);
  }
}
