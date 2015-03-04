// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <locale.h>
#include <uchar.h>

size_t mbrtoc16(char16_t *restrict pc16, const char *restrict s, size_t n,
                mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbrtoc16_l(pc16, s, n, ps, locale);
}
