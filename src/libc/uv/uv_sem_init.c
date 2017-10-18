// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <semaphore.h>
#include <uv.h>

int uv_sem_init(uv_sem_t *sem, unsigned int value) {
  return sem_init(sem, 0, value) == 0 ? 0 : -errno;
}
