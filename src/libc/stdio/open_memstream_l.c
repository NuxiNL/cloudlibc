// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <locale.h>
#include <stdio.h>

FILE *open_memstream_l(char **bufp, size_t *sizep, locale_t locale) {
  // TODO(ed): Implement.
  errno = ENOSYS;
  return NULL;
}
