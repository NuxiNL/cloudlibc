// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <string.h>

#define INDEX(c) ((unsigned char)(c) / LONG_BIT)
#define BIT(c) (1UL << ((unsigned char)(c) % LONG_BIT))

char *strtok_r(char *restrict s, const char *restrict sep,
               char **restrict lasts) {
  // Restore saved token.
  if (s == NULL) {
    s = *lasts;
    if (s == NULL)
      return NULL;
  }

  // Construct span bitmask.
  unsigned long span[(UCHAR_MAX + 1) / LONG_BIT] = {};
  while (*sep != '\0') {
    span[INDEX(*sep)] |= BIT(*sep);
    ++sep;
  }

  // Skip leading delimiters.
  while ((span[INDEX(*s)] & BIT(*s)) != 0)
    ++s;
  if (*s == '\0') {
    *lasts = NULL;
    return NULL;
  }

  // Find end of token.
  char *end = s + 1;
  span[INDEX('\0')] |= BIT('\0');
  while ((span[INDEX(*end)] & BIT(*end)) == 0)
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
