// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <wchar.h>

wchar_t *wcsdup(const wchar_t *string) {
  size_t len = wcslen(string) + 1;
  wchar_t *copy = malloc(len * sizeof(wchar_t));
  if (copy == NULL)
    return NULL;
  wmemcpy(copy, string, len);
  return copy;
}
