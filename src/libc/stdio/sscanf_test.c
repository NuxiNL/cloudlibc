// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <testing.h>

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
  char out[5];
  int after;
  ASSERT_EQ(1, sscanf("Hello", "%n%5c%n", &before, out, &after));
  ASSERT_EQ(0, before);
  ASSERT_ARREQ("Hello", out, 5);
  ASSERT_EQ(5, after);
}

TEST(sscanf, characters_malloc) {
  // Buffer should be allocated through malloc().
  char *out;
  ASSERT_EQ(1, sscanf("Hello", "%5mc", &out));
  ASSERT_ARREQ("Hello", out, 5);
  free(out);
}

TEST(sscanf, http_version) {
  // HTTP version number extraction performed by libevent's HTTP server.
  int major, minor;
  ASSERT_EQ(2, sscanf("HTTP/1.0", "HTTP/%d.%d%c", &major, &minor, NULL));
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

#if 0  // TODO(ed): Enable!
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

    ASSERT_EQ(2, sscanf("-12.8degrees Celcius", "%f%20s of %20s", &quant, units,
                        item));
    ASSERT_EQ(-12.8, quant);
    ASSERT_STREQ("degrees", units);

    ASSERT_EQ(0, sscanf("lots of luck", "%f%20s of %20s", &quant, units, item));

    ASSERT_EQ(3, sscanf("10.0LBS     of\ndirt", "%f%20s of %20s", &quant, units,
                        item));
    ASSERT_EQ(10.0, quant);
    ASSERT_STREQ("LBS", units);
    ASSERT_STREQ("dirt", item);

    ASSERT_EQ(
        0, sscanf("100ergs of energy", "%f%20s of %20s", &quant, units, item));
  }
#endif

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
