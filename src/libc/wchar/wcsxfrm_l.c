// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

size_t wcsxfrm_l(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n,
                 locale_t locale) {
  // TODO(ed): Implement.
  return wcslcpy(ws1, ws2, n);
}
