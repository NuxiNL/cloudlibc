// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#include "gtest/gtest.h"

TEST(swscanf, c11_examples) {
  // Example 1.
  {
    int i;
    float x;
    wchar_t name[50];
    ASSERT_EQ(3, swscanf(L"25 54.32E-1 thompson", L"%d%f%ls", &i, &x, name));
    ASSERT_EQ(25, i);
    ASSERT_EQ(5.432f, x);
    ASSERT_STREQ(L"thompson", name);
  }

  // Example 2.
  {
    int i;
    float x;
    double y;
    ASSERT_EQ(3, swscanf(L"56789 0123 56a72", L"%2d%f%*d %lf", &i, &x, &y));
    ASSERT_EQ(56, i);
    ASSERT_EQ(789.0f, x);
    ASSERT_EQ(56.0, y);
  }
}

TEST(swscanf, scanset) {
  // Positive matching.
  {
    wchar_t p1[8], p2;
    ASSERT_EQ(2, swscanf(L"Hello there!", L"%[Helo t]here%c", p1, &p2));
    ASSERT_STREQ(L"Hello t", p1);
    ASSERT_EQ('!', p2);

    ASSERT_EQ(1, swscanf(L"Hell[o] there!", L"%[][Helot]here%c", p1, &p2));
    ASSERT_STREQ(L"Hell[o]", p1);
    ASSERT_EQ(1, swscanf(L"Hello there!", L"%[Helo t]ere%c", p1, &p2));
    ASSERT_STREQ(L"Hello t", p1);
  }

  // Negative matching.
  {
    wchar_t p1[8], p2;
    ASSERT_EQ(2, swscanf(L"Hello there!", L"%[^]h]here%c", p1, &p2));
    ASSERT_STREQ(L"Hello t", p1);
    ASSERT_EQ('!', p2);

    ASSERT_EQ(1, swscanf(L"Hell[o] there!", L"%[^!@#)$(*# ]here%c", p1, &p2));
    ASSERT_STREQ(L"Hell[o]", p1);
    ASSERT_EQ(1, swscanf(L"Hello there!", L"%[^abcdfgh]ere%c", p1, &p2));
    ASSERT_STREQ(L"Hello t", p1);
  }
}
