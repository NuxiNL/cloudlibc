// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>
#include <common/stdio.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>

size_t fwrite(const void *restrict ptr, size_t size, size_t nitems,
              FILE *restrict stream) {
  // Check for overflow of size * nitems.
  flockfile_orientation(stream, -1);
  size_t writelen;
  if (mul_overflow(size, nitems, &writelen)) {
    stream->flags |= F_ERROR;
    funlockfile(stream);
    errno = EINVAL;
    return 0;
  }

  // Zero-sized write. Return immediately.
  if (writelen == 0) {
    funlockfile(stream);
    return 0;
  }

  // Write data.
  size_t len = fwrite_put(stream, ptr, writelen);
  funlockfile(stream);
  return len / size;
}
