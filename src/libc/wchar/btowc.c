// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wint_t btowc(int c) {
  if (c < 0 || c > 127)
    return WEOF;
  return c;
}
