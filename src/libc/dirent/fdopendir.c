// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

#include "dirent_impl.h"

#define DEFAULT_BUFFER_SIZE 4096

DIR *fdopendir(int fd) {
  // Allocate new directory object and read buffer.
  DIR *dirp = malloc(sizeof(*dirp));
  if (dirp == NULL)
    return NULL;
  dirp->buffer = malloc(DEFAULT_BUFFER_SIZE);
  if (dirp->buffer == NULL) {
    free(dirp);
    return NULL;
  }

  // Ensure that this is really a directory by already loading the first
  // chunk of data.
  cloudabi_errno_t error =
      cloudabi_sys_file_readdir(fd, dirp->buffer, DEFAULT_BUFFER_SIZE,
                                CLOUDABI_DIRCOOKIE_START, &dirp->buffer_used);
  if (error != 0) {
    free(dirp->buffer);
    free(dirp);
    errno = errno_fixup_directory(fd, error);
    return NULL;
  }

  // Initialize other members.
  dirp->fd = fd;
  dirp->cookie = CLOUDABI_DIRCOOKIE_START;
  dirp->buffer_processed = 0;
  dirp->buffer_size = DEFAULT_BUFFER_SIZE;
  dirp->dirent = NULL;
  dirp->dirent_size = 1;
  return dirp;
}
