// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libintl.h>
#include <locale.h>

#include "gtest/gtest.h"

TEST(dcngettext, example) {
  ASSERT_STREQ("%d bottles of beer on the wall",
               dcngettext("appname", "%d bottle of beer on the wall",
                          "%d bottles of beer on the wall", 0, LC_MESSAGES));
  ASSERT_STREQ("%d bottle of beer on the wall",
               dcngettext("appname", "%d bottle of beer on the wall",
                          "%d bottles of beer on the wall", 1, LC_MESSAGES));
  ASSERT_STREQ("%d bottles of beer on the wall",
               dcngettext("appname", "%d bottle of beer on the wall",
                          "%d bottles of beer on the wall", 2, LC_MESSAGES));
  ASSERT_STREQ("%d bottles of beer on the wall",
               dcngettext("appname", "%d bottle of beer on the wall",
                          "%d bottles of beer on the wall", 3, LC_MESSAGES));
}
