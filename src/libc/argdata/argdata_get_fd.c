// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

int argdata_get_fd(const argdata_t *ad, int *value) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_FD, &buf, &len);
      if (error != 0)
        return error;

      // Extract file descriptor number.
      uint32_t fd;
      error = parse_uint32(&fd, &buf, &len);
      if (error != 0)
        return error;

      // Validate file descriptor number.
      if (fd > INT_MAX)
        return EBADF;
      *value = fd;
      return 0;
    }
    default:
      return EINVAL;
  }
}
