// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/types.h>

#include <errno.h>
#include <wchar.h>

ssize_t getwdelim(wchar_t **restrict lineptr, size_t *restrict n,
                  wint_t delimiter, FILE *restrict stream) {
  // TODO(ed): Implement.
  errno = ENOSYS;
  return -1;
}
