// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <link.h>
#include <stddef.h>

#ifdef __arm__

void *dl_unwind_find_exidx(const void *pc, int *pcount) {
  // TODO(ed): Implement.
  *pcount = 0;
  return NULL;
}

#endif
