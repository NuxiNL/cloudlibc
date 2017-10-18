// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <testing.h>
#include <uv.h>

TEST(uv_key_create, example) {
  uv_key_t key;
  ASSERT_EQ(0, uv_key_create(&key));

  // Default value.
  ASSERT_EQ(NULL, uv_key_get(&key));

  // Change value.
  uv_key_set(&key, (void *)1);
  ASSERT_EQ((void *)1, uv_key_get(&key));

  uv_key_delete(&key);
}
