// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>

int feof(FILE *stream) {
  flockfile(stream);
  bool result = (stream->flags & F_EOF) != 0;
  funlockfile(stream);
  return result;
}
