// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/types.h>

#include <wchar.h>

ssize_t getwline(wchar_t **restrict lineptr, size_t *restrict n,
                 FILE *restrict stream) {
  return getwdelim(lineptr, n, L'\n', stream);
}
