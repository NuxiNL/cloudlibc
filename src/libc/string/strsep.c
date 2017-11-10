// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

char *strsep(char **lasts, const char *sep) {
  char *s = *lasts;
  if (s == NULL)
    return NULL;

  char *end = s + strcspn(s, sep);
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
