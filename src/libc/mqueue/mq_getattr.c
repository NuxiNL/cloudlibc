// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat) {
  // Fetch queue attributes.
  struct __mqd *mqd = mqdes.__mqd;
  pthread_mutex_lock(&mqd->lock);
  *mqstat = mqd->attr;
  pthread_mutex_unlock(&mqd->lock);
  return 0;
}
