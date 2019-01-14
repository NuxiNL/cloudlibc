// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/types.h>

#include <search.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(twalk, empty) {
  // Empty tree. Callback should not be invoked.
  posix_tnode *tree = nullptr;
  twalk(tree, (void (*)(const posix_tnode *, VISIT, int))42);
}

static int compar_int(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

#define NELEMENTS 1000
#define MAXTREEHEIGHT 15
static int numbers[NELEMENTS];

static size_t next_key = 0;
static int current_level = -1;

static void traverse(const posix_tnode *node, VISIT visit, int level) {
  ASSERT_GE(MAXTREEHEIGHT, level);
  switch (visit) {
    case preorder:
      ASSERT_EQ(++current_level, level);
      ASSERT_LE(&numbers[next_key], node->key);
      break;
    case postorder:
      ASSERT_EQ(current_level, level);
      ASSERT_EQ(&numbers[next_key++], node->key);
      break;
    case endorder:
      ASSERT_EQ(current_level--, level);
      ASSERT_GE(&numbers[next_key], node->key);
      break;
    case leaf:
      ASSERT_EQ(current_level + 1, level);
      ASSERT_EQ(&numbers[next_key++], node->key);
      break;
  }
}

TEST(twalk, example) {
  // The numbers to insert into the tree.
  for (size_t i = 0; i < NELEMENTS; ++i)
    numbers[i] = i;

  // Generate random order in which elements should be inserted.
  size_t insertion_order[NELEMENTS];
  for (size_t i = 0; i < NELEMENTS; ++i)
    insertion_order[i] = i;
  for (ssize_t i = NELEMENTS - 1; i > 0; --i) {
    size_t j = arc4random_uniform(i);
    int tmp = insertion_order[i];
    insertion_order[i] = insertion_order[j];
    insertion_order[j] = tmp;
  }

  // Insert all elements into the tree in random order.
  posix_tnode *root = NULL;
  for (size_t i = 0; i < NELEMENTS; ++i) {
    // Entry should not exist yet.
    int *keyp = &numbers[insertion_order[i]];
    ASSERT_EQ(NULL, tfind(keyp, &root, compar_int));
    ASSERT_EQ(NULL, tdelete(keyp, &root, compar_int));
    // Insertion should create new node in tree.
    ASSERT_EQ(keyp, tsearch(keyp, &root, compar_int)->key);
  }
  for (size_t i = 0; i < NELEMENTS; ++i) {
    // Successive searches should always return this node, even when
    // provided a different instance of the same key.
    int *keyp = &numbers[insertion_order[i]];
    int key = *keyp;
    ASSERT_EQ(keyp, tfind(&key, &root, compar_int)->key);
    ASSERT_EQ(keyp, tsearch(&key, &root, compar_int)->key);
  }

  // Invoke twalk() and check the order in which it iterates.
  twalk(root, traverse);
  ASSERT_EQ(NELEMENTS, next_key);
  ASSERT_EQ(-1, current_level);

  // Remove all elements from the tree.
  for (size_t i = 0; i < NELEMENTS; ++i) {
    int *keyp = &numbers[insertion_order[i]];
    int key = *keyp;
    ASSERT_NE(NULL, tdelete(&key, &root, compar_int));
    ASSERT_EQ(NULL, tdelete(&key, &root, compar_int));
    ASSERT_EQ(NULL, tfind(keyp, &root, compar_int));
  }
  ASSERT_EQ(NULL, root);
}
