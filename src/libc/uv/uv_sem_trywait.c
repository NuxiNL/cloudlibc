// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <semaphore.h>
#include <uv.h>

int uv_sem_trywait(uv_sem_t *sem) {
  return sem_trywait(sem) == 0 ? 0 : -errno;
}
