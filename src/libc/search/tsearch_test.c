// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdbool.h>
#include <stdlib.h>
#include <testing.h>

static int compar(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

TEST(tsearch, random) {
  // Create 1000 possible keys.
  int keys[1000];
  for (int i = 0; i < (int)__arraycount(keys); ++i)
    keys[i] = i;

  // Apply a million random operations on a binary tree and check the results.
  void *root = NULL;
  bool present[__arraycount(keys)] = {};
  for (int i = 0; i < 1000000; ++i) {
    int key = arc4random_uniform(__arraycount(keys));
    switch (arc4random_uniform(3)) {
    case 0: // tdelete().
      if (present[key]) {
        ASSERT_NE(NULL, tdelete(&key, &root, compar));
        present[key] = false;
      } else {
        ASSERT_EQ(NULL, tdelete(&key, &root, compar));
      }
      break;
    case 1: // tfind().
      if (present[key]) {
        ASSERT_EQ(&keys[key], *(int **)tfind(&key, &root, compar));
      } else {
        ASSERT_EQ(NULL, tfind(&key, &root, compar));
      }
      break;
    case 2: // tsearch().
      if (present[key]) {
        ASSERT_EQ(&keys[key], *(int **)tsearch(&key, &root, compar));
      } else {
        ASSERT_EQ(&keys[key], *(int **)tsearch(&keys[key], &root, compar));
        present[key] = true;
      }
      break;
    }
  }

  // Remove all entries from the tree.
  for (int key = 0; key < (int)__arraycount(keys); ++key)
    if (present[key])
      ASSERT_NE(NULL, tdelete(&key, &root, compar));
  ASSERT_EQ(NULL, root);
}
