// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
