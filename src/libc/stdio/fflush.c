// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_fflush(FILE *stream) {
  flockfile(stream);
  bool result = fop_flush(stream);
  if (result) {
    stream->ungetclen = 0;
    memset(&stream->readstate, '\0', sizeof(stream->readstate));
  }
  funlockfile(stream);
  return result ? 0 : EOF;
}

__weak_reference(__cloudlibc_fflush, fflush);
