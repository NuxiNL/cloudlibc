// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

// Makes a step to the left along the binary search tree. This step is
// also saved, so it can be replayed while rebalancing.
#define GO_LEFT()                                                       \
  do {                                                                  \
    if ((*leaf)->__balance == 0 ||                                      \
        ((*leaf)->__balance < 0 && (*leaf)->__right->__balance == 0)) { \
      /*                                                                \
       * If we reach a node that is balanced, or has a child in the     \
       * opposite direction that is balanced, we know that we won't     \
       * need to perform any rotations above this point. In this case   \
       * rotations are always capable of keeping the subtree in         \
       * balance. Make this the root node and reset the path.           \
       */                                                               \
      rootp = leaf;                                                     \
      path_init(&path);                                                 \
    }                                                                   \
    path_taking_left(&path);                                            \
    leaf = &(*leaf)->__left;                                            \
  } while (0)

// Makes a step to the right along the binary search tree.
#define GO_RIGHT()                                                     \
  do {                                                                 \
    if ((*leaf)->__balance == 0 ||                                     \
        ((*leaf)->__balance > 0 && (*leaf)->__left->__balance == 0)) { \
      rootp = leaf;                                                    \
      path_init(&path);                                                \
    }                                                                  \
    path_taking_right(&path);                                          \
    leaf = &(*leaf)->__right;                                          \
  } while (0)

// Deletes an object from an AVL tree if present.
//
// This algorithm is based on the non-recursive algorithm for
// AVL tree deletion by Neil Brown:
//
// http://neil.brown.name/blog/20041124101820
// http://neil.brown.name/blog/20041124141849
void *tdelete(const void *restrict key, posix_tnode **restrict rootp,
              int (*compar)(const void *, const void *)) {
  // POSIX requires that tdelete() returns NULL if rootp is NULL.
  if (rootp == NULL)
    return NULL;

  // Find the leaf that needs to be removed. Return if we cannot find an
  // existing entry. Keep track of the path that is taken to get to the
  // node, as we will need it to adjust the balances.
  posix_tnode *result = (posix_tnode *)1;
  struct path path;
  path_init(&path);
  posix_tnode **leaf = rootp;
  for (;;) {
    if (*leaf == NULL)
      return NULL;
    int cmp = compar(key, (*leaf)->key);
    if (cmp < 0) {
      result = *leaf;
      GO_LEFT();
    } else if (cmp > 0) {
      result = *leaf;
      GO_RIGHT();
    } else {
      break;
    }
  }

  // Found a matching key in the tree. Remove the node.
  if ((*leaf)->__left == NULL) {
    // Node has no left children. Replace it by its right subtree.
    posix_tnode *old = *leaf;
    *leaf = old->__right;
    free(old);
  } else {
    // Node has left children. Replace this node's key by its
    // predecessor's and remove that node instead.
    void **keyp = &(*leaf)->key;
    GO_LEFT();
    while ((*leaf)->__right != NULL)
      GO_RIGHT();
    posix_tnode *old = *leaf;
    *keyp = old->key;
    *leaf = old->__left;
    free(old);
  }

  // Walk along the same path a second time and adjust the balances.
  // Though this code looks similar to the rebalancing performed in
  // tsearch(), it is not identical. We now also need to consider the
  // case of outward imbalance in the right-right and left-left case
  // that only exists when deleting. Hence the duplication of code.
  for (posix_tnode **n = rootp; n != leaf;) {
    if (path_took_left(&path)) {
      posix_tnode *x = *n;
      if (x->__balance < 0) {
        posix_tnode *y = x->__right;
        if (y->__balance > 0) {
          // Right-left case.
          posix_tnode *z = y->__left;
          x->__right = z->__left;
          z->__left = x;
          y->__left = z->__right;
          z->__right = y;
          *n = z;

          x->__balance = z->__balance < 0 ? 1 : 0;
          y->__balance = z->__balance > 0 ? -1 : 0;
          z->__balance = 0;
        } else {
          // Right-right case.
          x->__right = y->__left;
          y->__left = x;
          *n = y;

          if (y->__balance < 0) {
            x->__balance = 0;
            y->__balance = 0;
          } else {
            x->__balance = -1;
            y->__balance = 1;
          }
        }
      } else {
        --x->__balance;
      }
      n = &x->__left;
    } else {
      posix_tnode *x = *n;
      if (x->__balance > 0) {
        posix_tnode *y = x->__left;
        if (y->__balance < 0) {
          // Left-right case.
          posix_tnode *z = y->__right;
          y->__right = z->__left;
          z->__left = y;
          x->__left = z->__right;
          z->__right = x;
          *n = z;

          x->__balance = z->__balance > 0 ? -1 : 0;
          y->__balance = z->__balance < 0 ? 1 : 0;
          z->__balance = 0;
        } else {
          // Left-left case.
          x->__left = y->__right;
          y->__right = x;
          *n = y;

          if (y->__balance > 0) {
            x->__balance = 0;
            y->__balance = 0;
          } else {
            x->__balance = 1;
            y->__balance = -1;
          }
        }
      } else {
        ++x->__balance;
      }
      n = &x->__right;
    }
  }

  // Return the parent of the old entry.
  return result;
}
