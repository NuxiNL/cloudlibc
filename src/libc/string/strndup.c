// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>
#include <cloudlibc_interceptors.h>

char *__cloudlibc_strndup(const char *s, size_t size) {
  size_t len = strnlen(s, size);
  char *copy = malloc(len + 1);
  if (copy == NULL)
    return NULL;
  memcpy(copy, s, len);
  copy[len] = '\0';
  return copy;
}

__weak_reference(__cloudlibc_strndup, strndup);
