// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

void hdestroy_r(struct hsearch_data *htab) {
  // Free underlying hash table.
  free(htab->__hsearch);
}
