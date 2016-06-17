// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

int mbsinit_l(const mbstate_t *ps, locale_t locale) {
  return mbsinit(ps);
}
