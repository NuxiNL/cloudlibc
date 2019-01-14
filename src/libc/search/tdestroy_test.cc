// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stdbool.h>
#include <stddef.h>

#include "gtest/gtest.h"

static int compar(const void *a, const void *b) {
  return (char *)a - (char *)b;
}

static void destructor(void *keyp) {
  bool *key = static_cast<bool *>(keyp);
  ASSERT_FALSE(*key);
  *key = true;
}

TEST(tdestroy, example) {
#define NKEYS 1000
  // Insert a thousand keys.
  bool keys[1000] = {};
  posix_tnode *root = NULL;
  for (size_t i = 0; i < NKEYS; ++i)
    ASSERT_EQ(&keys[i], tsearch(&keys[i], &root, compar)->key);

  // All of them should be destroyed if we call tdestroy().
  tdestroy(root, destructor);
  for (size_t i = 0; i < NKEYS; ++i)
    ASSERT_TRUE(keys[i]);
}
