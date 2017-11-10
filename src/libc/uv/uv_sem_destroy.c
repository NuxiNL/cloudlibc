// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <semaphore.h>
#include <stdlib.h>
#include <uv.h>

void uv_sem_destroy(uv_sem_t *sem) {
  if (sem_destroy(sem) != 0)
    abort();
}
