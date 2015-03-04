// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

size_t wcsspn(const wchar_t *ws1, const wchar_t *ws2) {
  const wchar_t *ws = ws1;
  for (;;) {
    const wchar_t *wt = ws2;
    for (;;) {
      if (*wt == L'\0')
        return ws - ws1;
      if (*ws == *wt++)
        break;
    }
    ++ws;
  }
}
