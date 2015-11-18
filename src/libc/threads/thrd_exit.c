// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <threads.h>

noreturn void thrd_exit(int res) {
  pthread_exit((void *)(intptr_t)res);
}
