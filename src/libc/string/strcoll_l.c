// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

int strcoll_l(const char *s1, const char *s2, locale_t locale) {
  // TODO(edje): Implement.
  return strcmp(s1, s2);
}
