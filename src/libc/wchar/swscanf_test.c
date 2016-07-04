// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <wchar.h>

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
    ASSERT_EQ( 3, swscanf(L"56789 0123 56a72", L"%2d%f%*d %lf", &i, &x, &y));
    ASSERT_EQ(56, i);
    ASSERT_EQ(789.0f, x);
    ASSERT_EQ(56.0, y);
  }
}
