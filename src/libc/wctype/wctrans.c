// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <wctype.h>

wctrans_t wctrans(const char *charclass) {
#define CHARCLASS(name)                   \
  if (strcmp(charclass, "to" #name) == 0) \
    return tow##name;
  CHARCLASS(lower);
  CHARCLASS(upper);
#undef CHARCLASS
  return NULL;
}
