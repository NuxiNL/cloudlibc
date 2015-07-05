// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

int wcscoll_l(const wchar_t *ws1, const wchar_t *ws2, locale_t locale) {
  // TODO(ed): Implement.
  return wcscmp(ws1, ws2);
}
