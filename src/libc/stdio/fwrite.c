// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <errno.h>
#include <stdint.h>
#include <stdio.h>

size_t fwrite(const void *restrict ptr, size_t size, size_t nitems,
              FILE *restrict stream) {
  // Zero-sized write. Return immediately.
  if (size == 0 || nitems == 0)
    return 0;

  // Check for overflow of size * nitems.
  if (nitems > SIZE_MAX / size) {
    flockfile(stream);
    stream->flags |= F_ERROR;
    funlockfile(stream);
    errno = EINVAL;
    return 0;
  }

  // Write data.
  flockfile(stream);
  size_t len = fwrite_put(stream, ptr, size * nitems);
  funlockfile(stream);
  return len / size;
}
