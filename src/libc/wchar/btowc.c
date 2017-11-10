// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <wchar.h>

wint_t btowc(int c) {
  if (c < 0 || c > SCHAR_MAX)
    return WEOF;
  return c;
}
