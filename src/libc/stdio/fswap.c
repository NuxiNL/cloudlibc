// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void fswap(FILE *stream1, FILE *stream2) {
  // Lock both objects.
  pthread_mutex_lock_pair_np(&stream1->lock, &stream2->lock);

  // Swap the contents. FILE_COPYSIZE is slightly smaller than
  // sizeof(FILE), as the lock itself is not swapped.
  FILE tmp;
  memcpy(&tmp, stream1, FILE_COPYSIZE);
  memcpy(stream1, stream2, FILE_COPYSIZE);
  memcpy(stream2, &tmp, FILE_COPYSIZE);

  pthread_mutex_unlock(&stream1->lock);
  pthread_mutex_unlock(&stream2->lock);
}
