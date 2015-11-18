// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <string.h>

char *strdup(const char *s) {
  size_t len = strlen(s) + 1;
  char *copy = malloc(len);
  if (copy == NULL)
    return NULL;
  memcpy(copy, s, len);
  return copy;
}
