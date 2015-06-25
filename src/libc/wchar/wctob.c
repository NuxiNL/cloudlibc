// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <wchar.h>

int wctob(wint_t c) {
  if (c < 0 || c > 127)
    return EOF;
  return c;
}
