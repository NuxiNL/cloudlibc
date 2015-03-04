// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>

#include "dirent_impl.h"

DIR *fdopendir(int fd) {
  // Allocate new directory object.
  DIR *dirp = malloc(sizeof(*dirp));
  if (dirp == NULL)
    return NULL;

  // Initialize members.
  pthread_mutex_init(&dirp->lock, NULL);
  dirp->buffer_processed = 0;
  dirp->fd = fd;
  dirp->cookie = CLOUDABI_DIRCOOKIE_START;

  // Ensure that this is really a directory by already loading the first
  // chunk of data.
  cloudabi_errno_t error =
      cloudabi_sys_file_readdir(dirp->fd, dirp->buffer, sizeof(dirp->buffer),
                                dirp->cookie, &dirp->buffer_length);
  if (error != 0) {
    pthread_mutex_destroy(&dirp->lock);
    free(dirp);
    errno = error;
    return NULL;
  }
  return dirp;
}
