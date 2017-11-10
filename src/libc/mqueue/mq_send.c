// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
            unsigned int msg_prio) {
  struct __mqd *mqd = mqdes.__mqd;
  if (!mq_send_pre(mqd, msg_len))
    return -1;
  while (mqd->attr.mq_curmsgs >= mqd->attr.mq_maxmsg)
    pthread_cond_wait(&mqd->cond_send, &mqd->lock);
  return mq_send_post(mqd, msg_ptr, msg_len, msg_prio);
}
