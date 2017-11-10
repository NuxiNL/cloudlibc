// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>

#include "mqueue_impl.h"

int mq_destroy(mqd_t mqdes) {
  // Destroy locking objects.
  struct __mqd *mqd = mqdes.__mqd;
  pthread_mutex_destroy(&mqd->lock);
  pthread_cond_destroy(&mqd->cond_receive);
  pthread_cond_destroy(&mqd->cond_send);

  // Free all pending messages.
  struct message *m = mqd->queue_receive;
  while (m != NULL) {
    struct message *m_next = m->next_receive;
    free(m);
    m = m_next;
  }

  // Free mqueue object itself.
  free(mqd);
  return 0;
}
