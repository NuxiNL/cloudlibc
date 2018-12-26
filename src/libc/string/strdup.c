// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strdup(const char *s) {
  size_t len = strlen(s) + 1;
  char *copy = malloc(len);
  if (copy == NULL)
    return NULL;
  memcpy(copy, s, len);
  return copy;
}

__weak_reference(__cloudlibc_strdup, strdup);
