// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

size_t strxfrm_l(char *restrict s1, const char *restrict s2, size_t n,
                 locale_t locale) {
  // TODO(ed): Implement.
  return strlcpy(s1, s2, n);
}
