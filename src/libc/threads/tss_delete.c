// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

void tss_delete(tss_t key) {
  pthread_key_delete(key);
}
