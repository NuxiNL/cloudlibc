// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libintl.h>
#include <testing.h>

TEST(gettext, example) {
  ASSERT_STREQ("Hello", gettext("Hello"));
}
