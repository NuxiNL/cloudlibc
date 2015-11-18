// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <threads.h>

void call_once(once_flag *flag, void (*func)(void)) {
  pthread_once(flag, func);
}
