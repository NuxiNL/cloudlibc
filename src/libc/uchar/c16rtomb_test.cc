// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <uchar.h>
#include <wchar.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(c16rtomb, ascii) {
  char c;
  mbstate_t mbs{};

  // Valid characters.
  ASSERT_EQ(1, c16rtomb(&c, u'A', &mbs));
  ASSERT_EQ('A', c);
  ASSERT_NE(0, mbsinit(&mbs));
  ASSERT_EQ(1, c16rtomb(&c, u'\0', &mbs));
  ASSERT_EQ('\0', c);
  ASSERT_NE(0, mbsinit(&mbs));

  // Characters cannot be expressed as ASCII.
  ASSERT_EQ((size_t)-1, c16rtomb(&c, u'€', &mbs));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_NE(0, mbsinit(&mbs));

  // Surrogate pair. Cannot be expressed as ASCII.
  ASSERT_EQ(0, c16rtomb(&c, 0xd801, &mbs));
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ((size_t)-1, c16rtomb(&c, 0xdc37, &mbs));
  ASSERT_EQ(EILSEQ, errno);
  ASSERT_EQ(0, mbsinit(&mbs));
}

TEST(c16rtomb, unicode) {
  char buf[MB_LEN_MAX];
  mbstate_t mbs{};

  // Valid characters.
  ASSERT_EQ(1, c16rtomb_l(buf, u'A', &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ('A', buf[0]);
  ASSERT_NE(0, mbsinit(&mbs));
  ASSERT_EQ(1, c16rtomb_l(buf, u'\0', &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ('\0', buf[0]);
  ASSERT_NE(0, mbsinit(&mbs));
  ASSERT_EQ(3, c16rtomb_l(buf, u'€', &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_THAT(buf, testing::StartsWith("€"));
  ASSERT_NE(0, mbsinit(&mbs));

  // Surrogate pair.
  ASSERT_EQ(0, c16rtomb_l(buf, 0xd801, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(0, mbsinit(&mbs));
  ASSERT_EQ(4, c16rtomb_l(buf, 0xdc37, &mbs, LC_C_UNICODE_LOCALE));
  ASSERT_THAT(buf, testing::StartsWith("𐐷"));
  ASSERT_NE(0, mbsinit(&mbs));
}
