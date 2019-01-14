// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdint.h>
#include <stdlib.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(wcstombs, bad) {
  // Non-ASCII character. String cannot be converted.
  ASSERT_EQ(-1, wcstombs(NULL, L"München", 42));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(wcstombs, zero) {
  // Output buffer has size zero. We should not access any memory.
  ASSERT_EQ(0, wcstombs((char *)0x42, L"Hello", 0));
}

TEST(wcstombs, length) {
  // Compute length of string, regardless of the length provided.
  ASSERT_EQ(11, wcstombs_l(NULL, L"Düsseldorf", 0, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(11, wcstombs_l(NULL, L"Düsseldorf", 5, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(11, wcstombs_l(NULL, L"Düsseldorf", 40, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(11, wcstombs_l(NULL, L"Düsseldorf", SIZE_MAX, LC_C_UNICODE_LOCALE));
}

TEST(wcstombs, convert) {
  // Convert the string.
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(0, wcstombs_l(buf, L"Düsseldorf", 0, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("AAAAAAAAAAAAA"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(1, wcstombs_l(buf, L"Düsseldorf", 1, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("DAAAAAAAAAAAA"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(1, wcstombs_l(buf, L"Düsseldorf", 2, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("DAAAAAAAAAAAA"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(3, wcstombs_l(buf, L"Düsseldorf", 3, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("DüAAAAAAAAAA"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(10, wcstombs_l(buf, L"Düsseldorf", 10, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("DüsseldorAAA"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(11, wcstombs_l(buf, L"Düsseldorf", 11, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("DüsseldorfAA"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(11, wcstombs_l(buf, L"Düsseldorf", 12, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("Düsseldorf\0A"));
  }
  {
    char buf[] = "AAAAAAAAAAAAA";
    ASSERT_EQ(11, wcstombs_l(buf, L"Düsseldorf", 13, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray("Düsseldorf\0A"));
  }
}
