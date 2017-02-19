// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "argdata_impl.h"

argdata_t *argdata_create_fd(int value) {
  // We should only refer to valid file descriptors.
  if (value < 0 || (uintmax_t)value > UINT32_MAX) {
    errno = EBADF;
    return NULL;
  }

  // Allocate object with space for encoded integer value.
  argdata_t *ad = malloc(sizeof(*ad) + sizeof(uint32_t) + 1);
  if (ad == NULL)
    return NULL;

  // Store digits. File descriptors are stored as fixed length, so that
  // they can be substituted without causing the binary representation
  // to change radically.
  uint8_t *bufstart = (uint8_t *)(ad + 1), *buf = bufstart;
  *buf++ = ADT_FD;
  encode_fd(value, &buf);

  ad->type = AD_BUFFER;
  ad->buffer = bufstart;
  ad->length = buf - bufstart;
  return ad;
}
