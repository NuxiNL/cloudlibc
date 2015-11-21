// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/queue.h>

#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>

#include "mqueue_impl.h"

int mq_close(mqd_t mqdes) {
  // Destroy locking objects.
  pthread_mutex_destroy(&mqdes->lock);
  pthread_cond_destroy(&mqdes->cond);

  // Free all pending messages.
  for (size_t i = 0; i < mqdes->heap_length; ++i) {
    struct message *m, *mtmp;
    STAILQ_FOREACH_SAFE(m, &mqdes->heap[i].messages, next, mtmp) {
      free(m);
    }
  }

  // Free heap and mqueue object itself.
  free(mqdes->heap);
  free(mqdes);
  return 0;
}
