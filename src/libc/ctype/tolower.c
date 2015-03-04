// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <ctype.h>

int tolower(int c) {
  if (c >= 'A' && c <= 'Z')
    return c - 'A' + 'a';
  return c;
}
