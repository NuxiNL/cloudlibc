// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/byteset.h>

#include <string.h>

char *strtok_r(char *restrict s, const char *restrict sep,
               char **restrict lasts) {
  // Restore saved token.
  if (s == NULL) {
    s = *lasts;
    if (s == NULL)
      return NULL;
  }

  // Construct span bitmask.
  byteset_t bs;
  byteemptyset(&bs);
  while (*sep != '\0') {
    byteaddset(&bs, *sep);
    ++sep;
  }

  // Skip leading delimiters.
  while (byteismember(&bs, *s))
    ++s;
  if (*s == '\0') {
    *lasts = NULL;
    return NULL;
  }

  // Find end of token.
  char *end = s + 1;
  byteaddset(&bs, '\0');
  while (!byteismember(&bs, *end))
    ++end;

  if (*end == '\0') {
    // Token is at the end of the input.
    *lasts = NULL;
  } else {
    // Token is not at the end of the input, so null terminate it.
    *end = '\0';
    *lasts = end + 1;
  }
  return s;
}
