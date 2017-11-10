// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

void call_once(once_flag *flag, void (*func)(void)) {
  pthread_once(flag, func);
}
