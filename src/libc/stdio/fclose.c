// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_fclose(FILE *stream) {
  // Close underlying descriptor.
  bool result = fop_close(stream);

  // Free file object and associated data.
  pthread_mutex_destroy(&stream->lock);
  free(stream);
  return result ? 0 : EOF;
}

__weak_reference(__cloudlibc_fclose, fclose);
