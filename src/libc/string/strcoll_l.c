// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

int strcoll_l(const char *s1, const char *s2, locale_t locale) {
  // TODO(ed): Implement.
  return strcmp(s1, s2);
}
