// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <string.h>

char *strndup(const char *s, size_t size) {
  size_t len = strnlen(s, size);
  char *copy = malloc(len + 1);
  if (copy == NULL)
    return NULL;
  memcpy(copy, s, len);
  copy[len] = '\0';
  return copy;
}
