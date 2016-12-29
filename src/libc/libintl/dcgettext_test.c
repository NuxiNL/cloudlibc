// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>
#include <locale.h>
#include <testing.h>

TEST(dcgettext, example) {
  ASSERT_STREQ("Hello", dcgettext("appname", "Hello", LC_MESSAGES));
}
