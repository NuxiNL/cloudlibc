// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <aio.h>

int aio_error(const struct aiocb *aiocbp) {
  return aiocbp->__aio_return < 0 ? -aiocbp->__aio_return : 0;
}
