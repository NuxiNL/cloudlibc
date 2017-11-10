// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <wchar.h>

#define BIT(c) (1UL << ((unsigned int)(c) % LONG_BIT))

size_t wcscspn(const wchar_t *ws1, const wchar_t *ws2) {
  // Construct span bloom filter.
  unsigned long span = 0;
  {
    const wchar_t *wt = ws2;
    do {
      span |= BIT(*wt);
    } while (*wt++ != L'\0');
  }

  // Scan over input. Only scan through the span string if the bloom
  // filter is positive.
  const wchar_t *ws = ws1;
  for (;;) {
    if ((span & BIT(*ws)) != 0) {
      const wchar_t *wt = ws2;
      do {
        if (*ws == *wt)
          return ws - ws1;
      } while (*wt++ != L'\0');
    }
    ++ws;
  }
}
