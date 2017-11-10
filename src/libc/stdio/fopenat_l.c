// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <unistd.h>

FILE *fopenat_l(int fd, const char *restrict pathname,
                const char *restrict mode, locale_t locale) {
  // Open file through openat().
  int oflags = get_oflags_from_string(mode);
  if (oflags == 0) {
    errno = EINVAL;
    return NULL;
  }
  int newfd = openat(fd, pathname, oflags, 0666);
  if (newfd == -1)
    return NULL;

  // Convert to FILE object.
  FILE *file = fdopen_l(newfd, mode, locale);
  if (file == NULL) {
    close(newfd);
    return NULL;
  }
  return file;
}
