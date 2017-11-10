// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <stdio.h>
#include <wchar.h>

int wctob(wint_t c) {
  if (c < 0 || c > SCHAR_MAX)
    return EOF;
  return c;
}
