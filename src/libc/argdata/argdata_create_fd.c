// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

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
  uint8_t *buf = (uint8_t *)(ad + 1);
  buf[0] = ADT_FD;
  buf[1] = value >> 24;
  buf[2] = value >> 16;
  buf[3] = value >> 8;
  buf[4] = value;

  ad->type = AD_BUFFER;
  ad->buffer = buf;
  ad->length = sizeof(uint32_t) + 1;
  return ad;
}
