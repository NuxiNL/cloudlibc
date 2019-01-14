// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <locale.h>
#include <stdint.h>
#include <stdlib.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(mbstowcs, bad) {
  // Non-ASCII character. String cannot be converted.
  ASSERT_EQ(-1, mbstowcs(NULL, "München", 42));
  ASSERT_EQ(EILSEQ, errno);
}

TEST(mbstowcs, zero) {
  // Output buffer has size zero. We should not access any memory.
  ASSERT_EQ(0, mbstowcs((wchar_t *)0x42, "Hello", 0));
}

TEST(mbstowcs, length) {
  // Compute length of string, regardless of the length provided.
  ASSERT_EQ(10, mbstowcs_l(NULL, "Düsseldorf", 0, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(10, mbstowcs_l(NULL, "Düsseldorf", 5, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(10, mbstowcs_l(NULL, "Düsseldorf", 40, LC_C_UNICODE_LOCALE));
  ASSERT_EQ(10, mbstowcs_l(NULL, "Düsseldorf", SIZE_MAX, LC_C_UNICODE_LOCALE));
}

TEST(mbstowcs, convert) {
  // Convert the string.
  {
    wchar_t buf[] = L"AAAAAAAAAAAA";
    ASSERT_EQ(0, mbstowcs_l(buf, "Düsseldorf", 0, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray(L"AAAAAAAAAAAA"));
  }
  {
    wchar_t buf[] = L"AAAAAAAAAAAA";
    ASSERT_EQ(4, mbstowcs_l(buf, "Düsseldorf", 4, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray(L"DüssAAAAAAAA"));
  }
  {
    wchar_t buf[] = L"AAAAAAAAAAAA";
    ASSERT_EQ(9, mbstowcs_l(buf, "Düsseldorf", 9, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray(L"DüsseldorAAA"));
  }
  {
    wchar_t buf[] = L"AAAAAAAAAAAA";
    ASSERT_EQ(10, mbstowcs_l(buf, "Düsseldorf", 10, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray(L"DüsseldorfAA"));
  }
  {
    wchar_t buf[] = L"AAAAAAAAAAAA";
    ASSERT_EQ(10, mbstowcs_l(buf, "Düsseldorf", 11, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray(L"Düsseldorf\0A"));
  }
  {
    wchar_t buf[] = L"AAAAAAAAAAAA";
    ASSERT_EQ(10, mbstowcs_l(buf, "Düsseldorf", 12, LC_C_UNICODE_LOCALE));
    ASSERT_THAT(buf, testing::ElementsAreArray(L"Düsseldorf\0A"));
  }
}
