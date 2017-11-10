// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <libgen.h>
#include <string.h>

char *dirname(char *path) {
  // If path is a null pointer or points to an empty string, dirname()
  // shall return a pointer to the string ".".
  if (path == NULL || *path == '\0')
    return (char *)".";

  // Find end of last pathname component.
  char *end = path + strlen(path);
  while (end > path + 1 && *(end - 1) == '/')
    --end;

  // Strip off the last pathname component.
  while (end > path && *(end - 1) != '/')
    --end;

  // If path does not contain a '/', then dirname() shall return a
  // pointer to the string ".".
  if (end == path) {
    path[0] = '.';
    path[1] = '\0';
    return path;
  }

  // Remove trailing slashes from the resulting directory name. Ensure
  // that at least one character remains.
  while (end > path + 1 && *(end - 1) == '/')
    --end;

  // Null terminate directory name and return it.
  *end = '\0';
  return path;
}
