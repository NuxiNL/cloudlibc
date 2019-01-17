// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>
#include <uv.h>

#include "gtest/gtest.h"

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
