// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
            unsigned int msg_prio) {
  if (!mq_send_pre(mqdes, msg_len))
    return -1;
  while (mqdes->attr.mq_curmsgs >= mqdes->attr.mq_maxmsg)
    pthread_cond_wait(&mqdes->cond, &mqdes->lock);
  return mq_send_post(mqdes, msg_ptr, msg_len, msg_prio);
}
