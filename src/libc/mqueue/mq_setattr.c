// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
  pthread_mutex_lock(&mqdes->lock);
  if (omqstat != NULL)
    *omqstat = mqdes->attr;
  mqdes->attr.mq_flags = mqstat->mq_flags;
  pthread_mutex_unlock(&mqdes->lock);
  return 0;
}
