// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "argdata_impl.h"

// File descriptors are stored as fixed length 32-bit numbers, so that
// they can be substituted without causing the binary representation to
// change radically. Simply use the value zero here, as the actual file
// descriptor number will be filled in when serializing.
static const uint8_t buf_fd[] = {ADT_FD, 0, 0, 0, 0};

static int fixed_fd(void *arg, size_t fd) {
  return (intptr_t)arg;
}

argdata_t *argdata_create_fd(int value) {
  // We should only refer to valid file descriptors.
  if (value < 0) {
    errno = EBADF;
    return NULL;
  }

  argdata_t *ad = malloc(sizeof(*ad));
  if (ad == NULL)
    return NULL;
  argdata_init_buffer(ad, buf_fd, sizeof(buf_fd), fixed_fd,
                      (void *)(intptr_t)value);
  return ad;
}
