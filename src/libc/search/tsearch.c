// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

static struct __tnode *tsearch_recurse(const void *key, struct __tnode *n,
                                       int (*compar)(const void *,
                                                     const void *),
                                       void **result) {
  if (n == NULL) {
    // Did not find a matching key in the tree. Allocate a new node.
    n = malloc(sizeof(*n));
    if (n == NULL) {
      *result = NULL;
      return NULL;
    }
    n->__key = (void *)key;
    n->__left = NULL;
    n->__right = NULL;
    n->__height = 1;
    *result = &n->__key;
    return n;
  } else {
    // Use the comparison function to traverse the tree.
    int cmp = compar(key, n->__key);
    if (cmp < 0) {
      n->__left = tsearch_recurse(key, n->__left, compar, result);
      return __tnode_rebalance(n);
    } else if (cmp > 0) {
      n->__right = tsearch_recurse(key, n->__right, compar, result);
      return __tnode_rebalance(n);
    } else {
      // Found an already existing entry with the same key.
      *result = &n->__key;
      return n;
    }
  }
}

void *tsearch(const void *key, void **rootp,
              int (*compar)(const void *, const void *)) {
  void *result;
  *rootp = tsearch_recurse(key, *rootp, compar, &result);
  return result;
}
