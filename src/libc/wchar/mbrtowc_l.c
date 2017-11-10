// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uchar.h>
#include <wchar.h>

size_t mbrtowc_l(wchar_t *restrict pwc, const char *restrict s, size_t n,
                 mbstate_t *restrict ps, locale_t locale) {
  return mbrtoc32_l((char32_t *)pwc, s, n, ps, locale);
}
