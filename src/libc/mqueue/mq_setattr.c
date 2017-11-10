// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>

#include "mqueue_impl.h"

int mq_setattr(mqd_t mqdes, const struct mq_attr *restrict mqstat,
               struct mq_attr *restrict omqstat) {
  // Only allow O_NONBLOCK to be set.
  if ((mqstat->mq_flags & ~O_NONBLOCK) != 0) {
    errno = EINVAL;
    return -1;
  }

  // Fetch old queue attributes and update flags.
  struct __mqd *mqd = mqdes.__mqd;
  pthread_mutex_lock(&mqd->lock);
  if (omqstat != NULL)
    *omqstat = mqd->attr;
  mqd->attr.mq_flags = mqstat->mq_flags;
  pthread_mutex_unlock(&mqd->lock);
  return 0;
}
