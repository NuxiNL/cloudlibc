// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>

#include "search_impl.h"

// Increases the balance of a node by one, as the result of an insertion
// in its left subtree. If the balance becomes more than 1, rotations
// are applied to restore balance. This function returns whether the
// resulting tree has increased in height.
static bool tsearch_balance_increase(struct __tnode **n) {
  struct __tnode *x = *n;
  if (x->__balance > 0) {
    struct __tnode *y = x->__left;
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
      struct __tnode *z = y->__right;
      y->__right = z->__left;
      z->__left = y;
      x->__left = z->__right;
      z->__right = x;

      x->__balance = z->__balance > 0 ? -1 : 0;
      y->__balance = z->__balance < 0 ? 1 : 0;
      z->__balance = 0;
      *n = z;
      return false;
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

      if (y->__balance > 0) {
        x->__balance = 0;
        y->__balance = 0;
        *n = y;
        return false;
      } else {
        x->__balance = 1;
        y->__balance = -1;
        *n = y;
        return true;
      }
    }
  } else {
    return ++x->__balance > 0;
  }
}

// Decreases the balance of a node by one, as the result of an insertion
// in its right subtree. If the balance becomes less than -1, rotations
// are applied to restore balance. This function returns whether the
// resulting tree has increased in height.
static bool tsearch_balance_decrease(struct __tnode **n) {
  struct __tnode *x = *n;
  if (x->__balance < 0) {
    struct __tnode *y = x->__right;
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
      struct __tnode *z = y->__left;
      x->__right = z->__left;
      z->__left = x;
      y->__left = z->__right;
      z->__right = y;

      x->__balance = z->__balance < 0 ? 1 : 0;
      y->__balance = z->__balance > 0 ? -1 : 0;
      z->__balance = 0;
      *n = z;
      return false;
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

      if (y->__balance < 0) {
        x->__balance = 0;
        y->__balance = 0;
        *n = y;
        return false;
      } else {
        x->__balance = -1;
        y->__balance = 1;
        *n = y;
        return true;
      }
    }
  } else {
    return --x->__balance < 0;
  }
}

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
             tsearch_balance_increase(n);
    } else if (cmp > 0) {
      return tsearch_recurse(key, &(*n)->__right, compar, result) &&
             tsearch_balance_decrease(n);
    } else {
      // Found an already existing entry with the same key.
      *result = &(*n)->__key;
      return false;
    }
  }
}

void *tsearch(const void *key, void **rootp,
              int (*compar)(const void *, const void *)) {
  void *result;
  tsearch_recurse(key, (struct __tnode **)rootp, compar, &result);
  tnode_assert(*rootp);
  return result;
}
