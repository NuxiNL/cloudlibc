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

  // Free all pending messages for every priority.
  struct priority *p, *ptmp;
  SLIST_FOREACH_SAFE(p, &mqdes->priorities, next, ptmp) {
    struct message *m, *mtmp;
    STAILQ_FOREACH_SAFE(m, &p->messages, next, mtmp) {
      free(m);
    }
    free(p);
  }

  // Free mqueue object itself.
  free(mqdes);
  return 0;
}
