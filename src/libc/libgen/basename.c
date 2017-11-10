// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libgen.h>
#include <string.h>

char *basename(char *path) {
  // If path is a null pointer or points to an empty string, basename()
  // shall return a pointer to the string ".".
  if (path == NULL || *path == '\0')
    return (char *)".";

  // Find end of last pathname component and null terminate it.
  char *ptr = path + strlen(path);
  while (ptr > path + 1 && *(ptr - 1) == '/')
    --ptr;
  *ptr-- = '\0';

  // Find beginning of last pathname component.
  while (ptr > path && *(ptr - 1) != '/')
    --ptr;
  return ptr;
}
