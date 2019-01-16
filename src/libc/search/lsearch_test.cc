// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stddef.h>
#include <iterator>

#include "gtest/gtest.h"

static int compar(const void *a, const void *b) {
  return *(int *)b - *(int *)a;
}

TEST(lsearch, example) {
  // Insert numbers into an array in sequential order.
  int v[10];
  for (int i = 0; i < (int)std::size(v); ++i) {
    // Number should not be present yet.
    size_t nel = i;
    ASSERT_EQ(NULL, lfind(&i, v, &nel, sizeof(int), compar));
    ASSERT_EQ(i, nel);

    // Insertion should add the number to the tail.
    ASSERT_EQ(&v[i], lsearch(&i, v, &nel, sizeof(int), compar));
    ASSERT_EQ(i + 1, nel);

    // Existing entries should still all be present.
    for (int j = 0; j <= i; ++j) {
      ASSERT_EQ(&v[j], lsearch(&j, v, &nel, sizeof(int), compar));
      ASSERT_EQ(i + 1, nel);
    }
  }
}
