// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#include "argdata_impl.h"

// In case no file descriptor conversion function is provided, simply
// disallow access to any of the process-local file descriptors.
static int disallow_fd_access(void *arg, size_t fd) {
  return -1;
}

argdata_t *argdata_from_buffer(const void *buf, size_t len,
                               int (*convert_fd)(void *, size_t),
                               void *convert_fd_arg) {
  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;
  argdata_init_buffer(ad, buf, len,
                      convert_fd != NULL ? convert_fd : disallow_fd_access,
                      convert_fd_arg);
  return ad;
}
