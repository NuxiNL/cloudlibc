// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <ctype.h>

int toupper(int c) {
  if (c >= 'a' && c <= 'z')
    return c - 'a' + 'A';
  return c;
}
