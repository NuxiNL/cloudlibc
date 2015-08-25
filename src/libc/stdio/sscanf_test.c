// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
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
  char ch;
  ASSERT_EQ(2, sscanf("HTTP/1.0", "HTTP/%d.%d%c", &major, &minor, &ch));
  ASSERT_EQ(1, major);
  ASSERT_EQ(0, minor);
}
