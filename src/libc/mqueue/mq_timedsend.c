// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
                 unsigned int msg_prio, const struct timespec *abstime) {
  struct __mqd *mqd = mqdes.__mqd;
  if (!mq_send_pre(mqd, msg_len))
    return -1;
  while (mqd->attr.mq_curmsgs >= mqd->attr.mq_maxmsg) {
    int error = pthread_cond_timedwait(&mqd->cond_send, &mqd->lock, abstime);
    if (error != 0) {
      pthread_mutex_unlock(&mqd->lock);
      errno = error;
      return -1;
    }
  }
  return mq_send_post(mqd, msg_ptr, msg_len, msg_prio);
}
