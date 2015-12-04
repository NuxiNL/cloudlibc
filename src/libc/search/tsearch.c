// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

void *tsearch(const void *key, void **rootp,
              int (*compar)(const void *, const void *)) {
  // Search the tree for an existing entry.
  struct __tnode **root = (struct __tnode **)rootp;
  while (*root != NULL) {
    int cmp = compar(key, (*root)->__key);
    if (cmp < 0)
      root = &(*root)->__left;
    else if (cmp > 0)
      root = &(*root)->__right;
    else
      return &(*root)->__key;
  }

  // Entry not found. Insert a new leaf node into the tree.
  // TODO(ed): Use a self-balancing binary search tree instead.
  *root = malloc(sizeof(**root));
  if (*root != NULL) {
    (*root)->__key = (void *)key;
    (*root)->__left = NULL;
    (*root)->__right = NULL;
  }
  return &(*root)->__key;
}
