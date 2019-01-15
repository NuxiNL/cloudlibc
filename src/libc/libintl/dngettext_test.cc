// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>

#include "gtest/gtest.h"

TEST(dngettext, example) {
  ASSERT_STREQ("%d bottles of beer on the wall",
               dngettext("appname", "%d bottle of beer on the wall",
                         "%d bottles of beer on the wall", 0));
  ASSERT_STREQ("%d bottle of beer on the wall",
               dngettext("appname", "%d bottle of beer on the wall",
                         "%d bottles of beer on the wall", 1));
  ASSERT_STREQ("%d bottles of beer on the wall",
               dngettext("appname", "%d bottle of beer on the wall",
                         "%d bottles of beer on the wall", 2));
  ASSERT_STREQ("%d bottles of beer on the wall",
               dngettext("appname", "%d bottle of beer on the wall",
                         "%d bottles of beer on the wall", 3));
}
