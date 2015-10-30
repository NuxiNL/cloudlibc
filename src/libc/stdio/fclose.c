// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fclose(FILE *stream) {
  // Close underlying descriptor.
  int result = fop_close(stream) ? 0 : EOF;

  // Free file object and associated data.
  pthread_mutex_destroy(&stream->lock);
  free(stream);
  return result;
}
