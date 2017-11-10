// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/mbstate.h>

#include <wchar.h>

int mbsinit(const mbstate_t *ps) {
  return mbstate_get_init(ps);
}
