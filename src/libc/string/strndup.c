// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
