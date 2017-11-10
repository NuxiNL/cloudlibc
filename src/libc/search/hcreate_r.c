// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

int hcreate_r(size_t nel, struct hsearch_data *htab) {
  // Allocate a hash table object. Ignore the provided hint and start
  // off with a table of sixteen entries. In most cases this hint is
  // just a wild guess. Resizing the table dynamically if the use
  // increases a threshold does not affect the worst-case running time.
  struct __hsearch *hsearch = malloc(sizeof(*hsearch));
  if (hsearch == NULL)
    return 0;
  hsearch->entries = calloc(16, sizeof(ENTRY));
  if (hsearch->entries == NULL) {
    free(hsearch);
    return 0;
  }

  // Pick a random initialization for the FNV-1a hashing. This makes it
  // hard to come up with a fixed set of keys to force hash collisions.
  arc4random_buf(&hsearch->offset_basis, sizeof(hsearch->offset_basis));
  hsearch->index_mask = 0xf;
  hsearch->entries_used = 0;
  htab->__hsearch = hsearch;
  return 1;
}
