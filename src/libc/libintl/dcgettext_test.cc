// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>
#include <locale.h>

#include "gtest/gtest.h"

TEST(dcgettext, example) {
  ASSERT_STREQ("Hello", dcgettext("appname", "Hello", LC_MESSAGES));
}
