// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdint.h>

#include "argdata_impl.h"

int argdata_get_fd(const argdata_t *ad, int *value) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer.buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_FD, &buf, &len);
      if (error != 0)
        return error;

      // Extract raw file descriptor number.
      uint32_t raw_fd;
      error = parse_fd(&raw_fd, &buf, &len);
      if (error != 0)
        return error;

      // Convert it to a process-local file descriptor number.
      int fd = ad->buffer.convert_fd(ad->buffer.convert_fd_arg, raw_fd);
      if (fd < 0)
        return EBADF;
      *value = fd;
      return 0;
    }
    default:
      return EINVAL;
  }
}
