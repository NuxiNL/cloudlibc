// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <wchar.h>

size_t mbsrtowcs_l(wchar_t *restrict dst, const char **restrict src, size_t len,
                   mbstate_t *restrict ps, locale_t locale) {
  return mbsnrtowcs_l(dst, src, SIZE_MAX, len, ps, locale);
}
