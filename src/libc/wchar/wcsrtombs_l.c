// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <wchar.h>

size_t wcsrtombs_l(char *restrict dst, const wchar_t **restrict src, size_t len,
                   mbstate_t *restrict ps, locale_t locale) {
  return wcsnrtombs_l(dst, src, SIZE_MAX, len, ps, locale);
}
