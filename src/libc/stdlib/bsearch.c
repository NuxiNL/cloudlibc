// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <stdlib.h>

void *bsearch(const void *key, const void *base, size_t nel, size_t width,
              int (*compar)(const void *, const void *)) {
  const char *basep = base;
  while (nel > 0) {
    // Pick pivot.
    size_t mid = nel / 2;
    const void *obj = basep + mid * width;

    int cmp = compar(key, obj);
    if (cmp < 0) {
      // key < obj. Restrict search to top of the list.
      nel = mid;
    } else if (cmp > 0) {
      // key > obj. Restrict search to bottom of the list.
      size_t skip = mid + 1;
      basep += skip * width;
      nel -= skip;
    } else {
      return (void *)obj;
    }
  }
  return NULL;
}
