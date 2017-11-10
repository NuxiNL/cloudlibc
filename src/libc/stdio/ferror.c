// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>

int ferror(FILE *stream) {
  flockfile(stream);
  bool result = (stream->flags & F_ERROR) != 0;
  funlockfile(stream);
  return result;
}
