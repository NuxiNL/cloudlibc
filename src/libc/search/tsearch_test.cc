// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(tsearch, null) {
  // NULL root pointer is also allowed.
  ASSERT_EQ(NULL, tdelete(NULL, NULL, NULL));
  ASSERT_EQ(NULL, tfind(NULL, NULL, NULL));
  ASSERT_EQ(NULL, tsearch(NULL, NULL, NULL));
}

// Validates the integrity of an AVL tree.
static inline unsigned int tnode_assert(const posix_tnode *n) {
  if (n == NULL)
    return 0;
  unsigned int height_left = tnode_assert(n->__left);
  unsigned int height_right = tnode_assert(n->__right);
  int balance = (int)height_left - (int)height_right;
  EXPECT_LE(-1, balance);
  EXPECT_GE(1, balance);
  EXPECT_EQ(balance, n->__balance);
  return (height_left > height_right ? height_left : height_right) + 1;
}

static int compar(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

TEST(tsearch, random) {
  // Create 1000 possible keys.
  int keys[1000];
  for (int i = 0; i < (int)std::size(keys); ++i)
    keys[i] = i;

  // Apply random operations on a binary tree and check the results.
  posix_tnode *root = NULL;
  bool present[std::size(keys)] = {};
  for (int i = 0; i < 10000; ++i) {
    int key = arc4random_uniform(std::size(keys));
    switch (arc4random_uniform(3)) {
      case 0:  // tdelete().
        if (present[key]) {
          ASSERT_NE(NULL, tdelete(&key, &root, compar));
          present[key] = false;
        } else {
          ASSERT_EQ(NULL, tdelete(&key, &root, compar));
        }
        break;
      case 1:  // tfind().
        if (present[key]) {
          ASSERT_EQ(&keys[key], tfind(&key, &root, compar)->key);
        } else {
          ASSERT_EQ(NULL, tfind(&key, &root, compar));
        }
        break;
      case 2:  // tsearch().
        if (present[key]) {
          ASSERT_EQ(&keys[key], tsearch(&key, &root, compar)->key);
        } else {
          ASSERT_EQ(&keys[key], tsearch(&keys[key], &root, compar)->key);
          present[key] = true;
        }
        break;
    }
    tnode_assert(root);
  }

  // Remove all entries from the tree.
  for (int key = 0; key < (int)std::size(keys); ++key)
    if (present[key])
      ASSERT_NE(NULL, tdelete(&key, &root, compar));
  ASSERT_EQ(NULL, root);
}
