// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
                 unsigned int msg_prio, const struct timespec *abstime) {
  if (!mq_send_pre(mqdes, msg_len))
    return -1;
  while (mqdes->attr.mq_curmsgs >= mqdes->attr.mq_maxmsg) {
    int error = pthread_cond_timedwait(&mqdes->cond, &mqdes->lock, abstime);
    if (error != 0) {
      pthread_mutex_unlock(&mqdes->lock);
      errno = error;
      return -1;
    }
  }
  return mq_send_post(mqdes, msg_ptr, msg_len, msg_prio);
}
