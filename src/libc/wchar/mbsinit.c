// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/mbstate.h>

#include <wchar.h>

int mbsinit(const mbstate_t *ps) {
  return mbstate_get_init(ps);
}
