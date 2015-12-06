// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef SEARCH_SEARCH_IMPL_H
#define SEARCH_SEARCH_IMPL_H

#include <assert.h>
#include <search.h>
#include <stdbool.h>
#include <stddef.h>

// Increases the balance of a node by one, as the result of an insertion
// in its left subtree. If the balance becomes more than 1, rotations
// are applied to restore balance. This function returns whether the
// resulting tree has increased in height.
static inline bool tnode_balance_increase(struct __tnode **n) {
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
static inline bool tnode_balance_decrease(struct __tnode **n) {
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

// Validates the integrity of an AVL tree.
static inline unsigned int tnode_assert(const struct __tnode *n) {
#if 0
  if (n == NULL)
    return 0;
  unsigned int height_left = tnode_assert(n->__left);
  unsigned int height_right = tnode_assert(n->__right);
  int balance = (int)height_left - (int)height_right;
  assert(balance >= -1 && balance <= 1);
  assert(balance == n->__balance);
  return (height_left > height_right ? height_left : height_right) + 1;
#else
  return 0;
#endif
}

#endif
