// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

int hcreate_r(size_t nel, struct hsearch_data *htab) {
  // Allocate a hash table object. Ignore the provided hint and start
  // off with a table of sixteen entries.
  struct __hsearch *hsearch = hsearch_alloc(16);
  if (hsearch == NULL)
    return 0;

  // Pick a random initialization for the FNV-1a hashing. This makes it
  // hard to come up with a fixed set of keys to force hash collisions.
  arc4random_buf(&hsearch->offset_basis, sizeof(hsearch->offset_basis));
  hsearch->entries_used = 0;
  htab->__hsearch = hsearch;
  return 1;
}
