// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <semaphore.h>
#include <uv.h>

int uv_sem_init(uv_sem_t *sem, unsigned int value) {
  return sem_init(sem, 0, value) == 0 ? 0 : -errno;
}
