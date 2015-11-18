// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

DIR *opendirat(int dir, const char *dirname) {
  // Open directory.
  int fd = openat(dir, dirname, O_RDONLY | O_NONBLOCK | O_DIRECTORY);
  if (fd == -1)
    return NULL;

  // Create directory handle.
  DIR *result = fdopendir(fd);
  if (result == NULL)
    close(fd);
  return result;
}
