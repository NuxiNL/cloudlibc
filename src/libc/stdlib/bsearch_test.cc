// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "gtest/gtest.h"

static int int_compar(const void *a, const void *b) {
  return *(const int *)a - *(const int *)b;
}

TEST(bsearch, match) {
  int list[20] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
                  10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  for (int listsize = 1; listsize < 20; ++listsize) {
    SCOPED_TRACE(listsize);
    for (int key = 0; key < listsize; ++key) {
      SCOPED_TRACE(key);
      ASSERT_EQ(&list[key],
                bsearch(&key, &list, listsize, sizeof(int), int_compar));
    }
  }
}

TEST(bsearch, no_match) {
  int key = 42;
  int list[5] = {0, 1, 2, 3, 4};
  ASSERT_EQ(NULL, bsearch(&key, &list, 5, sizeof(int), int_compar));
}
