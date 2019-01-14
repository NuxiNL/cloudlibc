// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <search.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gtest/gtest.h"

#define NKEYS 1000
#define KEYCHARS 4

TEST(hsearch_r, random) {
  // Prepare the keys.
  char keys[NKEYS][KEYCHARS];
  for (unsigned int i = 0; i < NKEYS; ++i)
    snprintf(keys[i], sizeof(keys[i]), "%d", i);
  char *data = NULL;

  // Create the hash table.
  struct hsearch_data htab;
  ASSERT_NE(0, hcreate_r(NKEYS, &htab));

  // Apply random hsearch_r() operations.
  bool inserted[NKEYS] = {};
  for (size_t i = 0; i < NKEYS * 10; ++i) {
    unsigned int key = arc4random_uniform(NKEYS);
    switch (arc4random_uniform(2)) {
      case 0: {  // ENTER.
        if (inserted[key]) {
          char keystr[KEYCHARS];
          snprintf(keystr, sizeof(keystr), "%d", key);
          ENTRY entry = {.key = keystr};
          ENTRY *retval;
          ASSERT_NE(0, hsearch_r(entry, ENTER, &retval, &htab));
          ASSERT_EQ(keys[key], retval->key);
          ASSERT_EQ(data + key, retval->data);
        } else {
          ENTRY entry = {.key = keys[key], .data = data + key};
          ENTRY *retval;
          ASSERT_NE(0, hsearch_r(entry, ENTER, &retval, &htab));
          ASSERT_EQ(keys[key], retval->key);
          ASSERT_EQ(data + key, retval->data);
          inserted[key] = true;
        }
        break;
      }
      case 1: {  // FIND.
        char keystr[KEYCHARS];
        snprintf(keystr, sizeof(keystr), "%d", key);
        ENTRY entry = {.key = keystr};
        if (inserted[key]) {
          ENTRY *retval;
          ASSERT_NE(0, hsearch_r(entry, FIND, &retval, &htab));
          ASSERT_EQ(keys[key], retval->key);
          ASSERT_EQ(data + key, retval->data);
        } else {
          ASSERT_EQ(0, hsearch_r(entry, FIND, NULL, &htab));
          ASSERT_EQ(ESRCH, errno);
        }
        break;
      }
    }
  }

  hdestroy_r(&htab);
}
