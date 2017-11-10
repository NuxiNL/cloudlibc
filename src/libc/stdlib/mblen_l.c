// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdlib.h>

int mblen_l(const char *s, size_t n, locale_t locale) {
  return mbtowc_l(NULL, s, n, locale);
}
