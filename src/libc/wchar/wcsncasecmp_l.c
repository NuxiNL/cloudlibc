// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

int wcsncasecmp_l(const wchar_t *ws1, const wchar_t *ws2, size_t n,
                  locale_t locale) {
  return wcsncasecmp(ws1, ws2, n);
}
