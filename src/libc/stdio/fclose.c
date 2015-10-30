// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fclose(FILE *stream) __no_lock_analysis {
  // Flush and close underlying descriptor.
  int result = 0;
  if ((stream->oflags & O_WRONLY) != 0 && !fop_write_flush(stream))
    result = EOF;
  if (stream->fd >= 0 && close(stream->fd) == -1)
    result = EOF;

  // Free file object and associated data.
  pthread_mutex_destroy(&stream->lock);
  // TODO(ed): Clean up storage associated with FILE object.
  free(stream);
  return result;
}
