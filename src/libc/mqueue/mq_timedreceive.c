// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

ssize_t mq_timedreceive(mqd_t mqdes, char *restrict msg_ptr, size_t msg_len,
                        unsigned int *restrict msg_prio,
                        const struct timespec *restrict abstime) {
  struct __mqd *mqd = mqdes.__mqd;
  if (!mq_receive_pre(mqd, msg_len))
    return -1;
  while (mqd->attr.mq_curmsgs <= 0) {
    int error = pthread_cond_timedwait(&mqd->cond_receive, &mqd->lock, abstime);
    if (error != 0) {
      pthread_mutex_unlock(&mqd->lock);
      errno = error;
      return -1;
    }
  }
  return mq_receive_post(mqd, msg_ptr, msg_prio);
}
