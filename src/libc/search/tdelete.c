// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>

#include "search_impl.h"

// Extracts the key of the largest node from a tree, while maintaining
// its balance.
static bool tdelete_extract_largest(struct __tnode **np, void **result) {
  struct __tnode *n = *np;
  if (n->__right == NULL) {
    // Found the maximum node. Extract the key and delete the node.
    *result = n->__key;
    *np = n->__left;
    free(n);
    return true;
  } else {
    // Extract the largest node from the right subtree and rebalance.
    return tdelete_extract_largest(&n->__right, result) &&
           !tnode_balance_increase(np);
  }
}

static bool tdelete_recurse(const void *key, struct __tnode **n,
                            int (*compar)(const void *, const void *),
                            void **result) {
  if (*n == NULL) {
    // Did not find a matching key in the tree.
    *result = NULL;
    return false;
  } else {
    // Use the comparison function to traverse the tree.
    int cmp = compar(key, (*n)->__key);
    if (cmp < 0) {
      *result = &(*n)->__key;
      return tdelete_recurse(key, &(*n)->__left, compar, result) &&
             !tnode_balance_decrease(n);
    } else if (cmp > 0) {
      *result = &(*n)->__key;
      return tdelete_recurse(key, &(*n)->__right, compar, result) &&
             !tnode_balance_increase(n);
    } else {
      // Found a matching node to delete.
      if ((*n)->__left == NULL) {
        // No left child. Replace the node by the right subtree.
        struct __tnode *newroot = (*n)->__right;
        free(*n);
        *n = newroot;
        return true;
      } else {
        // Replace this node's key by its predecessor's and deallocate
        // that node instead.
        return tdelete_extract_largest(&(*n)->__left, &(*n)->__key) &&
               !tnode_balance_decrease(n);
      }
    }
  }
}

void *tdelete(const void *restrict key, void **restrict rootp,
              int (*compar)(const void *, const void *)) {
  if (rootp == NULL)
    return NULL;
  struct __tnode *root = *rootp;
  void *result = (void *)1;
  tdelete_recurse(key, &root, compar, &result);
  *rootp = root;
  return result;
}
