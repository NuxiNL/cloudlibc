// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

ssize_t mq_timedreceive(mqd_t mqdes, char *restrict msg_ptr, size_t msg_len,
                        unsigned int *restrict msg_prio,
                        const struct timespec *restrict abstime) {
  if (!mq_receive_pre(mqdes, msg_len))
    return -1;
  while (mqdes->attr.mq_curmsgs <= 0) {
    int error = pthread_cond_timedwait(&mqdes->cond, &mqdes->lock, abstime);
    if (error != 0) {
      pthread_mutex_unlock(&mqdes->lock);
      errno = error;
      return -1;
    }
  }
  return mq_receive_post(mqdes, msg_ptr, msg_prio);
}
