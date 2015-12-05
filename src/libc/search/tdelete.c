// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

// Extracts the largest node from a tree, while maintaining its balance.
static struct __tnode *tdelete_extract_largest(struct __tnode *n,
                                               struct __tnode **result) {
  if (n->__right == NULL) {
    // Found the maximum node. Replace it by its left subtree.
    *result = n;
    return n->__left;
  } else {
    // Extract the largest node from the right subtree and rebalance.
    n->__right = tdelete_extract_largest(n->__right, result);
    return __tnode_rebalance(n);
  }
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
      // Found a matching node to delete. Free the entry.
      struct __tnode *left = n->__left, *right = n->__right;
      free(n);
      if (left == NULL) {
        // No left child. Replace the node by the right subtree.
        return right;
      } else if (right == NULL) {
        // No right child. Replace the node by the left subtree.
        return left;
      } else {
        // Let the largest node of the left subtree be the new root.
        struct __tnode *newroot;
        left = tdelete_extract_largest(left, &newroot);
        newroot->__left = left;
        newroot->__right = right;
        return __tnode_rebalance(newroot);
      }
    }
  }
}

void *tdelete(const void *restrict key, void **restrict rootp,
              int (*compar)(const void *, const void *)) {
  void *result = (void *)1;
  *rootp = tdelete_recurse(key, *rootp, compar, &result);
  return result;
}
