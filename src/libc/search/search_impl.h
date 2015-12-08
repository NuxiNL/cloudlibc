// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef SEARCH_SEARCH_IMPL_H
#define SEARCH_SEARCH_IMPL_H

#include <limits.h>
#include <search.h>
#include <stdbool.h>

// The maximum depth of a tree. An AVL tree's height is less than
// log(sqrt(5) * (n + 2) / log(phi) - 2. As n can never exceed the
// number of bytes in the address space, we can state that an AVL tree
// will never be higher than 46 nodes on a 32-bit system or 92 nodes on
// a 64-bit system. Approximate this bound by simply multiplying the
// number of bits in a pointer by 1.5.
//
// This constant is used to store parent links in an array when
// inserting and deleting nodes. These need to be tracked to perform
// rebalancing. This array is 768 bytes on a 64-bit system. Though this
// may sound like a lot at first, a simple recursive implementation
// could easily reach such memory usage for smaller trees.
#define TNODE_HEIGHT_MAX (sizeof(void *) * CHAR_BIT * 3 / 2)

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
      *n = z;

      x->__balance = z->__balance > 0 ? -1 : 0;
      y->__balance = z->__balance < 0 ? 1 : 0;
      z->__balance = 0;
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
      *n = y;

      if (y->__balance > 0) {
        x->__balance = 0;
        y->__balance = 0;
        return false;
      } else {
        x->__balance = 1;
        y->__balance = -1;
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
      *n = z;

      x->__balance = z->__balance < 0 ? 1 : 0;
      y->__balance = z->__balance > 0 ? -1 : 0;
      z->__balance = 0;
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
      *n = y;

      if (y->__balance < 0) {
        x->__balance = 0;
        y->__balance = 0;
        return false;
      } else {
        x->__balance = -1;
        y->__balance = 1;
        return true;
      }
    }
  } else {
    return --x->__balance < 0;
  }
}

#endif
