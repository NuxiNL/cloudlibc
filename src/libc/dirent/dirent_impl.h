// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef DIRENT_DIRENT_IMPL_H
#define DIRENT_DIRENT_IMPL_H

#include <common/syscalldefs.h>

#include <stddef.h>

struct dirent;

#define DIRENT_DEFAULT_BUFFER_SIZE 4096

struct _DIR {
  // Directory file descriptor and cookie.
  int fd;
  cloudabi_dircookie_t cookie;

  // Read buffer.
  char *buffer;
  size_t buffer_processed;
  size_t buffer_size;
  size_t buffer_used;

  // Object returned by readdir().
  struct dirent *dirent;
  size_t dirent_size;
};

#endif
