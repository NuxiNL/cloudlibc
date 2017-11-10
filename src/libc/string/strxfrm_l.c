// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

size_t strxfrm_l(char *restrict s1, const char *restrict s2, size_t n,
                 locale_t locale) {
  // TODO(ed): Implement.
  return strlcpy(s1, s2, n);
}
