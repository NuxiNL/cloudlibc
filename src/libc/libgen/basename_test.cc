// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libgen.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(basename, null) {
  ASSERT_STREQ(".", basename(NULL));
}

TEST(basename, example) {
#define EXAMPLE(in, out)                \
  {                                     \
    char inbuf[] = in;                  \
    ASSERT_STREQ(out, basename(inbuf)); \
  }
  EXAMPLE("", ".");
  EXAMPLE("foo/", "foo");
  EXAMPLE("//foo//", "foo");

  // Examples from POSIX.
  EXAMPLE("/usr/lib", "lib");
  EXAMPLE("/usr/", "usr");
  EXAMPLE("/", "/");
  EXAMPLE("///", "/");
  EXAMPLE("//usr//lib//", "lib");
#undef EXAMPLE
}
