// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <ctype.h>

int isascii(int c) {
  return c >= 0 && c <= 127;
}
