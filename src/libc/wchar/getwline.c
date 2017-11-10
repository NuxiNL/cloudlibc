// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/types.h>

#include <wchar.h>

ssize_t getwline(wchar_t **restrict lineptr, size_t *restrict n,
                 FILE *restrict stream) {
  return getwdelim(lineptr, n, L'\n', stream);
}
