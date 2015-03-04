// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>

void setbuf(FILE *restrict stream, char *restrict buf) {
  setvbuf(stream, buf, buf != NULL ? _IOFBF : _IONBF, BUFSIZ);
}
