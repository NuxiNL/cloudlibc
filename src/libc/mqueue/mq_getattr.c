// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat) {
  // Fetch queue attributes.
  pthread_mutex_lock(&mqdes->lock);
  *mqstat = mqdes->attr;
  pthread_mutex_unlock(&mqdes->lock);
  return 0;
}
