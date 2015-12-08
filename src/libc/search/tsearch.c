// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>

#include "search_impl.h"

void *tsearch(const void *key, void **rootp,
              int (*compar)(const void *, const void *)) {
  // POSIX requires that tsearch() returns NULL if rootp is NULL.
  if (rootp == NULL)
    return NULL;
  struct __tnode *root = *rootp;

  // Find the location in the tree where the new key needs to be
  // inserted. Return if we've found an existing entry. Keep track of
  // all of the parents for rebalancing.
  struct __tnode **parents[TNODE_HEIGHT_MAX];
  size_t nparents = 0;
  struct __tnode **n = &root;
  while (*n != NULL) {
    int cmp = compar(key, (*n)->__key);
    if (cmp < 0) {
      parents[nparents++] = n;
      n = &(*n)->__left;
    } else if (cmp > 0) {
      parents[nparents++] = n;
      n = &(*n)->__right;
    } else {
      return &(*n)->__key;
    }
  }

  // Did not find a matching key in the tree. Insert a new node.
  struct __tnode *result = *n = malloc(sizeof(**n));
  if (result == NULL)
    return NULL;
  result->__key = (void *)key;
  result->__left = NULL;
  result->__right = NULL;
  result->__balance = 0;

  // Perform rebalancing as long as the height of the subtree for a
  // given parent has increased. Depending on whether the subtree is the
  // left or right child of the parent, the balance needs to be
  // increased or decreased.
  while (nparents-- > 0) {
    struct __tnode **parent = parents[nparents];
    if (n == &(*parent)->__left) {
      if (!tnode_balance_increase(parent))
        break;
    } else {
      if (!tnode_balance_decrease(parent))
        break;
    }
    n = parent;
  }
  *rootp = root;
  return result;
}
