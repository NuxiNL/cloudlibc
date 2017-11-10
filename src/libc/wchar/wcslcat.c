// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

size_t wcslcat(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  // Skip the part of the destination buffer that is used.
  size_t skipped = 0;
  while (n > 0 && *ws1 != '\0') {
    ++ws1;
    --n;
    ++skipped;
  }

  // Copy up to n - 1 wchar_tacters into the destination buffer.
  const wchar_t *begin = ws2;
  while (n > 1) {
    *ws1 = *ws2;
    if (*ws2 == '\0')
      return ws2 - begin + skipped;
    ++ws1;
    ++ws2;
    --n;
  }

  // Nul-terminate the destination buffer if space is available.
  if (n > 0)
    *ws1 = '\0';

  // Continue computing the length of ws2.
  while (*ws2 != '\0')
    ++ws2;
  return ws2 - begin + skipped;
}
