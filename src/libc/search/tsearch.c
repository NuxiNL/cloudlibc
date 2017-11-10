// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stdlib.h>

#include "search_impl.h"

// Inserts an object into an AVL tree if not present.
//
// This algorithm is based on the non-recursive algorithm for
// AVL tree insertion by Neil Brown:
//
// http://neil.brown.name/blog/20041124101820
// http://neil.brown.name/blog/20041124141849
posix_tnode *tsearch(const void *key, posix_tnode **rootp,
                     int (*compar)(const void *, const void *)) {
  // POSIX requires that tsearch() returns NULL if rootp is NULL.
  if (rootp == NULL)
    return NULL;

  // Find the leaf where the new key needs to be inserted. Return if
  // we've found an existing entry. Keep track of the path that is taken
  // to get to the node, as we will need it to adjust the balances.
  struct path path;
  path_init(&path);
  posix_tnode **leaf = rootp;
  while (*leaf != NULL) {
    if ((*leaf)->__balance != 0) {
      // If we reach a node that has a non-zero balance on the way, we
      // know that we won't need to perform any rotations above this
      // point. In this case rotations are always capable of keeping the
      // subtree in balance. Make this the root node and reset the path.
      rootp = leaf;
      path_init(&path);
    }
    int cmp = compar(key, (*leaf)->key);
    if (cmp < 0) {
      path_taking_left(&path);
      leaf = &(*leaf)->__left;
    } else if (cmp > 0) {
      path_taking_right(&path);
      leaf = &(*leaf)->__right;
    } else {
      return *leaf;
    }
  }

  // Did not find a matching key in the tree. Insert a new node.
  posix_tnode *result = *leaf = malloc(sizeof(**leaf));
  if (result == NULL)
    return NULL;
  result->key = (void *)key;
  result->__left = NULL;
  result->__right = NULL;
  result->__balance = 0;

  // Walk along the same path a second time and adjust the balances.
  // Except for the first node, all of these nodes must have a balance
  // of zero, meaning that these nodes will not get out of balance.
  for (posix_tnode *n = *rootp; n != *leaf;) {
    if (path_took_left(&path)) {
      n->__balance += 1;
      n = n->__left;
    } else {
      n->__balance -= 1;
      n = n->__right;
    }
  }

  // Adjusting the balances may have pushed the balance of the root node
  // out of range. Perform a rotation to bring the balance back in range.
  posix_tnode *x = *rootp;
  if (x->__balance > 1) {
    posix_tnode *y = x->__left;
    if (y->__balance < 0) {
      // Left-right case.
      //
      //         x
      //        / \            z
      //       y   D          / \
      //      / \     -->    y   x
      //     A   z          /|   |\
      //        / \        A B   C D
      //       B   C
      posix_tnode *z = y->__right;
      y->__right = z->__left;
      z->__left = y;
      x->__left = z->__right;
      z->__right = x;
      *rootp = z;

      x->__balance = z->__balance > 0 ? -1 : 0;
      y->__balance = z->__balance < 0 ? 1 : 0;
      z->__balance = 0;
    } else {
      // Left-left case.
      //
      //        x           y
      //       / \         / \
      //      y   C  -->  A   x
      //     / \             / \
      //    A   B           B   C
      x->__left = y->__right;
      y->__right = x;
      *rootp = y;

      x->__balance = 0;
      y->__balance = 0;
    }
  } else if (x->__balance < -1) {
    posix_tnode *y = x->__right;
    if (y->__balance > 0) {
      // Right-left case.
      //
      //       x
      //      / \              z
      //     A   y            / \
      //        / \   -->    x   y
      //       z   D        /|   |\
      //      / \          A B   C D
      //     B   C
      posix_tnode *z = y->__left;
      x->__right = z->__left;
      z->__left = x;
      y->__left = z->__right;
      z->__right = y;
      *rootp = z;

      x->__balance = z->__balance < 0 ? 1 : 0;
      y->__balance = z->__balance > 0 ? -1 : 0;
      z->__balance = 0;
    } else {
      // Right-right case.
      //
      //       x               y
      //      / \             / \
      //     A   y    -->    x   C
      //        / \         / \
      //       B   C       A   B
      x->__right = y->__left;
      y->__left = x;
      *rootp = y;

      x->__balance = 0;
      y->__balance = 0;
    }
  }

  // Return the new entry.
  return result;
}
