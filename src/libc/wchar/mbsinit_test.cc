// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(mbsinit, init) {
  // A null pointer should correspond with the initial state.
  ASSERT_NE(0, mbsinit(NULL));

  // A zeroed object should also correspond with the initial state.
  mbstate_t initial_mbstate{};
  ASSERT_NE(0, mbsinit(&initial_mbstate));

  // The interaction of mbsinit() with other functions is tested in
  // other unit tests.
}
