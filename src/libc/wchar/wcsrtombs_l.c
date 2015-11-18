// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <wchar.h>

size_t wcsrtombs_l(char *restrict dst, const wchar_t **restrict src, size_t len,
                   mbstate_t *restrict ps, locale_t locale) {
  return wcsnrtombs_l(dst, src, SIZE_MAX, len, ps, locale);
}
