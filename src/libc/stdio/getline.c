// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

ssize_t getline(char **restrict lineptr, size_t *restrict n,
                FILE *restrict stream) {
  return getdelim(lineptr, n, '\n', stream);
}
