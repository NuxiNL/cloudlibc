// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>

ssize_t getline(char **restrict lineptr, size_t *restrict n,
                FILE *restrict stream) {
  return getdelim(lineptr, n, '\n', stream);
}
