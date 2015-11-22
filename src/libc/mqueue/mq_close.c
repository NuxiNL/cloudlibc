// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>

#include "mqueue_impl.h"

int mq_close(mqd_t mqdes) {
  // Destroy locking objects.
  pthread_mutex_destroy(&mqdes->lock);
  pthread_cond_destroy(&mqdes->cond);

  // Free all pending messages.
  struct message *m = mqdes->queue_receive;
  while (m != NULL) {
    struct message *m_next = m->next_receive;
    free(m);
    m = m_next;
  }

  // Free mqueue object itself.
  free(mqdes);
  return 0;
}
