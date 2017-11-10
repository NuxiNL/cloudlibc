// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

int strcoll_l(const char *s1, const char *s2, locale_t locale) {
  // TODO(ed): Implement.
  return strcmp(s1, s2);
}
