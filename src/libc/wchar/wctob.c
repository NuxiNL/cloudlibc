// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <stdio.h>
#include <wchar.h>

int wctob(wint_t c) {
  if (c < 0 || c > SCHAR_MAX)
    return EOF;
  return c;
}
