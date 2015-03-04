// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef DIRENT_DIRENT_IMPL_H
#define DIRENT_DIRENT_IMPL_H

#include <common/syscalldefs.h>

#include <sys/types.h>

#include <dirent.h>
#include <limits.h>

struct _DIR {
  // Per-directory handle lock.
  pthread_mutex_t lock;

  // Read buffer.
  union {
    char buffer[NAME_MAX * 20];
    cloudabi_dirent_t first_dirent;
  };
  size_t buffer_length;
  size_t buffer_processed;

  // Directory file descriptor and cookie.
  int fd;
  cloudabi_dircookie_t cookie;

  // Object returned by readdir().
  struct dirent dirent;
};

#endif
