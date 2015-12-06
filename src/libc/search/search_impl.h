// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef SEARCH_SEARCH_IMPL_H
#define SEARCH_SEARCH_IMPL_H

#include <assert.h>
#include <search.h>

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
