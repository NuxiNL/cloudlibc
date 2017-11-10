// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/types.h>

#include <errno.h>
#include <wchar.h>

ssize_t getwdelim(wchar_t **restrict lineptr, size_t *restrict n,
                  wint_t delimiter, FILE *restrict stream) {
  // TODO(ed): Implement.
  errno = ENOSYS;
  return -1;
}
