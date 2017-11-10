// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

void clearerr(FILE *stream) {
  flockfile(stream);
  stream->flags &= ~(F_EOF | F_ERROR);
  funlockfile(stream);
}
