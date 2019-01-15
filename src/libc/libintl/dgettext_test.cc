// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#include "gtest/gtest.h"

TEST(dgettext, example) {
  ASSERT_STREQ("Hello", dgettext("appname", "Hello"));
}
