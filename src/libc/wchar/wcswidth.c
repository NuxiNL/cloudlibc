// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

int wcswidth(const wchar_t *pwcs, size_t n) {
  int len = 0;
  while (n-- > 0 && *pwcs != L'\0') {
    int l = wcwidth(*pwcs++);
    if (l == -1)
      return -1;
    len += l;
  }
  return len;
}
