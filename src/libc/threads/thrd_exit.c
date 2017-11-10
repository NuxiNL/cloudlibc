// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <threads.h>

noreturn void thrd_exit(int res) {
  pthread_exit((void *)(intptr_t)res);
}
