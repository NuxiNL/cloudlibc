// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <ctype.h>

int isprint(int c) {
  return c >= ' ' && c <= '~';
}
