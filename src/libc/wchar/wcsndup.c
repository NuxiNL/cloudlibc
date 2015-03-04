// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>
#include <wchar.h>

wchar_t *wcsndup(const wchar_t *string, size_t size) {
  size_t len = wcsnlen(string, size);
  wchar_t *copy = malloc((len + 1) * sizeof(wchar_t));
  if (copy == NULL)
    return NULL;
  wmemcpy(copy, string, len);
  copy[len] = '\0';
  return copy;
}
