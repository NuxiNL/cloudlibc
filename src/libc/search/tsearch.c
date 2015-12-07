// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>

#include "search_impl.h"

static bool tsearch_recurse(const void *key, struct __tnode **n,
                            int (*compar)(const void *, const void *),
                            void **result) {
  if (*n == NULL) {
    // Did not find a matching key in the tree. Allocate a new node.
    *n = malloc(sizeof(**n));
    if (*n == NULL) {
      *result = NULL;
      return false;
    }
    (*n)->__key = (void *)key;
    (*n)->__left = NULL;
    (*n)->__right = NULL;
    (*n)->__balance = 0;
    *result = &(*n)->__key;
    return true;
  } else {
    // Use the comparison function to traverse the tree.
    int cmp = compar(key, (*n)->__key);
    if (cmp < 0) {
      return tsearch_recurse(key, &(*n)->__left, compar, result) &&
             tnode_balance_increase(n);
    } else if (cmp > 0) {
      return tsearch_recurse(key, &(*n)->__right, compar, result) &&
             tnode_balance_decrease(n);
    } else {
      // Found an already existing entry with the same key.
      *result = &(*n)->__key;
      return false;
    }
  }
}

void *tsearch(const void *key, void **rootp,
              int (*compar)(const void *, const void *)) {
  if (rootp == NULL)
    return NULL;
  void *result;
  tsearch_recurse(key, (struct __tnode **)rootp, compar, &result);
  return result;
}
