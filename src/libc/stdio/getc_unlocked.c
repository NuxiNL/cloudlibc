// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int getc_unlocked(FILE *stream) {
  // Obtain the read buffer.
  const char *readbuf;
  size_t readbuflen;
  if (!fread_peek(stream, &readbuf, &readbuflen) || readbuflen == 0)
    return EOF;

  // Consume a single character.
  unsigned char ch = *readbuf;
  fread_consume(stream, 1);
  return ch;
}
