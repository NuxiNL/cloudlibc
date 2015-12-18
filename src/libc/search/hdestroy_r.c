// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

void hdestroy_r(struct hsearch_data *htab) {
  // Free hash table object and its entries.
  struct __hsearch *hsearch = htab->__hsearch;
  free(hsearch->entries);
  free(hsearch);
}
