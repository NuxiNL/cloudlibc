// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <aio.h>
#include <errno.h>

int lio_listio(int mode, struct aiocb *restrict const *restrict list, int nent,
               ...) {
  // TODO(ed): Implement.
  errno = ENOSYS;
  return -1;
}
