// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdint.h>

int argdata_get_fd(const argdata_t *ad, int *value) {
  // Extract file descriptor number.
  int fd;
  switch (ad->type) {
    case AD_BUFFER:
      if (ad->buffer.len != 5 || ad->buffer.buf[0] != ADT_FD)
        return EINVAL;
      fd = (uint_least32_t)ad->buffer.buf[1] << 24 |
           (uint_least24_t)ad->buffer.buf[2] << 16 |
           (uint_least16_t)ad->buffer.buf[3] << 8 | ad->buffer.buf[4];
      break;
    default:
      return EINVAL;
  }

  // Validate file descriptor number.
  if (fd < 0)
    return EBADF;
  *value = fd;
  return 0;
}
