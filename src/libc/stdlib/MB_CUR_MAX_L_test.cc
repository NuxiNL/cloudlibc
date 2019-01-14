// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(MB_CUR_MAX_L, examples) {
  ASSERT_EQ(1, MB_CUR_MAX_L(LC_C_LOCALE));
  ASSERT_EQ(4, MB_CUR_MAX_L(LC_C_UNICODE_LOCALE));
}
