// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(sscanf, null) {
  // Empty format string means we should not process any input.
  ASSERT_EQ(0, sscanf(NULL, ""));
}

TEST(sscanf, numbers1) {
  unsigned int value1;
  unsigned char value2;

  ASSERT_EQ(1, sscanf("123", "%u", &value1));
  ASSERT_EQ(123, value1);

  ASSERT_EQ(2, sscanf("      123456", "%3u%4hhu", &value1, &value2));
  ASSERT_EQ(123, value1);
  ASSERT_EQ(255, value2);
}

TEST(sscanf, characters) {
  // Parse 5 characters at once.
  int before;
  char out[6];
  int after;
  ASSERT_EQ(1, sscanf("Hello", "%n%5c%n", &before, out, &after));
  ASSERT_EQ(0, before);
  out[5] = '\0';
  ASSERT_STREQ("Hello", out);
  ASSERT_EQ(5, after);
}

TEST(sscanf, characters_malloc) {
  // Buffer should be allocated through malloc().
  char *out;
  ASSERT_EQ(1, sscanf("Hello", "%5mc", &out));
  ASSERT_THAT(out, testing::StartsWith("Hello"));
  free(out);
}

TEST(sscanf, http_version) {
  // HTTP version number extraction performed by libevent's HTTP server.
  int major, minor;
  ASSERT_EQ(2,
            sscanf("HTTP/1.0", "HTTP/%d.%d%c", &major, &minor, (char *)NULL));
  ASSERT_EQ(1, major);
  ASSERT_EQ(0, minor);
}

TEST(sscanf, numbered_arguments) {
  int value1, value2;
  ASSERT_EQ(2, sscanf("12345 67890", "%1$d%2$d", &value1, &value2));
  ASSERT_EQ(12345, value1);
  ASSERT_EQ(67890, value2);

  ASSERT_EQ(2, sscanf("23456 78901", "%2$d%1$d", &value1, &value2));
  ASSERT_EQ(78901, value1);
  ASSERT_EQ(23456, value2);
}

TEST(sscanf, c11_examples) {
  // Example 1.
  {
    int i;
    float x;
    char name[50];
    ASSERT_EQ(3, sscanf("25 54.32E-1 thompson", "%d%f%s", &i, &x, name));
    ASSERT_EQ(25, i);
    ASSERT_EQ(5.432f, x);
    ASSERT_STREQ("thompson", name);
  }

  // Example 2.
  {
    int i;
    float x;
    char name[50];
    ASSERT_EQ(
        3, sscanf("56789 0123 56a72", "%2d%f%*d %[0123456789]", &i, &x, name));
    ASSERT_EQ(56, i);
    ASSERT_EQ(789.0f, x);
    ASSERT_STREQ("56", name);
  }

  // Example 3.
  {
    float quant;
    char units[21], item[21];
    ASSERT_EQ(3,
              sscanf("2 quarts of oil", "%f%20s of %20s", &quant, units, item));
    ASSERT_EQ(2, quant);
    ASSERT_STREQ("quarts", units);
    ASSERT_STREQ("oil", item);

    ASSERT_EQ(2, sscanf("-12.5degrees Celcius", "%f%20s of %20s", &quant, units,
                        item));
    ASSERT_EQ(-12.5, quant);
    ASSERT_STREQ("degrees", units);

    ASSERT_EQ(0, sscanf("lots of luck", "%f%20s of %20s", &quant, units, item));

    ASSERT_EQ(3, sscanf("10.0LBS     of\ndirt", "%f%20s of %20s", &quant, units,
                        item));
    ASSERT_EQ(10.0, quant);
    ASSERT_STREQ("LBS", units);
    ASSERT_STREQ("dirt", item);

    // For some reason, the standard requires that this fails to parse,
    // but our floating point literal parser is smart enough to only
    // parse the "100" part.
    ASSERT_EQ(
        3, sscanf("100ergs of energy", "%f%20s of %20s", &quant, units, item));
    ASSERT_EQ(100, quant);
    ASSERT_STREQ("ergs", units);
    ASSERT_STREQ("energy", item);
  }

  // Example 4.
  {
    int d1, d2 = 12345, n1, n2;
    ASSERT_EQ(1, sscanf("123", "%d%n%n%d", &d1, &n1, &n2, &d2));
    ASSERT_EQ(123, d1);
    ASSERT_EQ(3, n1);
    ASSERT_EQ(3, n2);
    ASSERT_EQ(12345, d2);
  }

  // Example 5.
  // WG14's N2033: %% should always skip leading whitespace. For literal
  // characters, this should only be done when also preceded by
  // whitespace.
  {
    int i;
    ASSERT_EQ(0, sscanf("foo  %  bar  42", "foo%%bar%d", &i));
    ASSERT_EQ(1, sscanf("foo  %  bar  42", "foo%% bar%d", &i));
    ASSERT_EQ(42, i);
  }
}

TEST(sscanf, scanset) {
  // Positive matching.
  {
    char p1[8], p2;
    ASSERT_EQ(2, sscanf("Hello there!", "%[Helo t]here%c", p1, &p2));
    ASSERT_STREQ("Hello t", p1);
    ASSERT_EQ('!', p2);

    ASSERT_EQ(1, sscanf("Hell[o] there!", "%[][Helot]here%c", p1, &p2));
    ASSERT_STREQ("Hell[o]", p1);
    ASSERT_EQ(1, sscanf("Hello there!", "%[Helo t]ere%c", p1, &p2));
    ASSERT_STREQ("Hello t", p1);
  }

  // Negative matching.
  {
    char p1[8], p2;
    ASSERT_EQ(2, sscanf("Hello there!", "%[^]h]here%c", p1, &p2));
    ASSERT_STREQ("Hello t", p1);
    ASSERT_EQ('!', p2);

    ASSERT_EQ(1, sscanf("Hell[o] there!", "%[^!@#)$(*# ]here%c", p1, &p2));
    ASSERT_STREQ("Hell[o]", p1);
    ASSERT_EQ(1, sscanf("Hello there!", "%[^abcdfgh]ere%c", p1, &p2));
    ASSERT_STREQ("Hello t", p1);
  }
}

TEST(sscanf, whitespace) {
  // U+2028 should not be considered a whitespace character by default.
  char str1[20], str2[20];
  ASSERT_EQ(1, sscanf("Hello\xe2\x80\xa8World", "%s%s", str1, str2));
  ASSERT_STREQ("Hello\xe2\x80\xa8World", str1);

  // Using an UTF-8 locale should cause the string to be split.
  ASSERT_EQ(2, sscanf_l("Hello\xe2\x80\xa8World", LC_C_UNICODE_LOCALE, "%s%s",
                        str1, str2));
  ASSERT_STREQ("Hello", str1);
  ASSERT_STREQ("World", str2);
}
