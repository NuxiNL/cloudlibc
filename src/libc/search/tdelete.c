// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

// Recombines two children of a node into a new subtree. This operation
// is needed when removing a node from the tree that still has children.
static struct __tnode *tdelete_recombine(struct __tnode *left,
                                         struct __tnode *right) {
  // If the node only has one child, there is nothing to recombine.
  // Simply return the node directly.
  if (left == NULL)
    return right;
  if (right == NULL)
    return left;

  // Node has two children. Continue recombining on the right hand side
  // of the left child.
  left->__right = tdelete_recombine(left->__right, right);
  return __tnode_rebalance(left);
}

static struct __tnode *tdelete_recurse(const void *key, struct __tnode *n,
                                       int (*compar)(const void *,
                                                     const void *),
                                       void **result) {
  if (n == NULL) {
    // Did not find a matching key in the tree.
    *result = NULL;
    return NULL;
  } else {
    // Use the comparison function to traverse the tree.
    int cmp = compar(key, n->__key);
    if (cmp < 0) {
      *result = &n->__key;
      n->__left = tdelete_recurse(key, n->__left, compar, result);
      return __tnode_rebalance(n);
    } else if (cmp > 0) {
      *result = &n->__key;
      n->__right = tdelete_recurse(key, n->__right, compar, result);
      return __tnode_rebalance(n);
    } else {
      // Found a matching node to delete. Free the entry and replace it
      // by a subtree that does not contain the node.
      struct __tnode *left = n->__left, *right = n->__right;
      free(n);
      return tdelete_recombine(left, right);
    }
  }
}

void *tdelete(const void *restrict key, void **restrict rootp,
              int (*compar)(const void *, const void *)) {
  void *result = (void *)1;
  *rootp = tdelete_recurse(key, *rootp, compar, &result);
  return result;
}
