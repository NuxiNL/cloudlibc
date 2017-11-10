// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline size_t roundup2(size_t lowerbound) {
  // Never allocate a buffer smaller than 16 bytes.
  size_t value = 16;
  while (value < lowerbound)
    value *= 2;
  return value;
}

static inline bool append(FILE *stream, char **restrict lineptr,
                          size_t *restrict n, size_t *restrict nread,
                          const char *restrict readbuf, size_t readbuflen)
    __requires_exclusive(*stream) {
  // Grow buffer if needed.
  if (*n < *nread + readbuflen) {
    size_t newsize = roundup2(*nread + readbuflen);
    char *newlineptr = realloc(*lineptr, newsize);
    if (newlineptr == NULL) {
      stream->flags |= F_ERROR;
      return false;
    }
    *lineptr = newlineptr;
    *n = newsize;
  }

  // Copy data to output buffer.
  memcpy(*lineptr + *nread, readbuf, readbuflen);
  *nread += readbuflen;
  return true;
}

ssize_t getdelim(char **restrict lineptr, size_t *restrict n, int delimiter,
                 FILE *restrict stream) {
  // Buffer and length pointer should be non-zero.
  flockfile_orientation(stream, -1);
  if (lineptr == NULL || n == NULL) {
    stream->flags |= F_ERROR;
    funlockfile(stream);
    errno = EINVAL;
    return -1;
  }

  // Line length is zero when no buffer is provided.
  if (*lineptr == NULL)
    *n = 0;

  size_t nread = 0;
  for (;;) {
    const char *readbuf;
    size_t readbuflen;
    if (!fread_peek(stream, &readbuf, &readbuflen) ||
        (readbuflen == 0 && nread == 0)) {
      // I/O failure, or end-of-file without returning any data.
      funlockfile(stream);
      return -1;
    }

    // End-of-file. Return data up to end-of-file.
    if (readbuflen == 0)
      break;

    const char *match = memchr(readbuf, delimiter, readbuflen);
    if (match != NULL) {
      // Found delimiter.
      size_t consume = match - readbuf + 1;
      if (!append(stream, lineptr, n, &nread, readbuf, consume)) {
        funlockfile(stream);
        return -1;
      }
      fread_consume(stream, consume);
      break;
    } else {
      // Delimiter not found. Consume entire buffer.
      if (!append(stream, lineptr, n, &nread, readbuf, readbuflen)) {
        funlockfile(stream);
        return -1;
      }
      fread_consume(stream, readbuflen);
    }
  }

  // Append trailing null character.
  char null = '\0';
  if (!append(stream, lineptr, n, &nread, &null, 1)) {
    funlockfile(stream);
    return -1;
  }
  funlockfile(stream);
  return nread - 1;
}
