// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <aio.h>
#include <errno.h>

int aio_write(struct aiocb *aiocbp) {
  // Alter opcode to LIO_WRITE and schedule it using lio_listio().
  aiocbp->aio_lio_opcode = LIO_WRITE;
  if (lio_listio(LIO_NOWAIT, &aiocbp, 1) == -1) {
    // Return error code of the individual I/O operation if available.
    if (errno == EIO)
      errno = aio_return(aiocbp);
    return -1;
  }
  return 0;
}
