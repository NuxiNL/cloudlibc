// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>

#include "search_impl.h"

void *tdelete(const void *restrict key, void **restrict rootp,
              int (*compar)(const void *, const void *)) {
  // POSIX requires that tdelete() returns NULL if rootp is NULL.
  if (rootp == NULL)
    return NULL;
  struct __tnode *root = *rootp;

  // Find the location in the tree of the node that needs to be removed.
  // Return if we can't find an existing entry. Keep track of all of the
  // parents for rebalancing.
  struct __tnode **parents[TNODE_HEIGHT_MAX];
  size_t nparents = 0;
  void *result = (void *)1;
  struct __tnode **n = &root;
  for (;;) {
    if (*n == NULL)
      return NULL;
    int cmp = compar(key, (*n)->__key);
    if (cmp < 0) {
      parents[nparents++] = n;
      result = *n;
      n = &(*n)->__left;
    } else if (cmp > 0) {
      parents[nparents++] = n;
      result = *n;
      n = &(*n)->__right;
    } else {
      break;
    }
  }

  // Found a matching key in the tree. Remove the node.
  if ((*n)->__left == NULL) {
    // Node has no left children. Replace it by its right subtree.
    struct __tnode *old = *n;
    *n = old->__right;
    free(old);
  } else {
    // Node has left children. Replace this node's key by its
    // predecessor's and remove that node instead.
    void **keyp = &(*n)->__key;
    parents[nparents++] = n;
    result = *n;
    n = &(*n)->__left;
    while ((*n)->__right != NULL) {
      parents[nparents++] = n;
      result = *n;
      n = &(*n)->__right;
    }
    struct __tnode *old = *n;
    *keyp = old->__key;
    *n = old->__left;
    free(old);
  }

  // Perform rebalancing as long as the height of the subtree for a
  // given parent has decreased. Depending on whether the subtree is the
  // left or right child of the parent, the balance needs to be
  // decreased or increased.
  while (nparents-- > 0) {
    struct __tnode **parent = parents[nparents];
    if (n == &(*parent)->__left) {
      if (tnode_balance_decrease(parent))
        break;
    } else {
      if (tnode_balance_increase(parent))
        break;
    }
    n = parent;
  }
  *rootp = root;
  return result;
}
