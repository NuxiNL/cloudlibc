// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdlib.h>

#include "mqueue_impl.h"

mqd_t mq_open_anon(const struct mq_attr *attr) {
  // Only allow O_NONBLOCK to be set. Maximum number of messages and
  // message size must be positive.
  if ((attr->mq_flags & ~O_NONBLOCK) != 0 || attr->mq_maxmsg <= 0 ||
      attr->mq_msgsize <= 0) {
    errno = EINVAL;
    return (mqd_t)-1;
  }

  // Allocate and initialize a new message queue object.
  mqd_t mqdes = malloc(sizeof(*mqdes));
  if (mqdes == NULL)
    return (mqd_t)-1;
  if (pthread_mutex_init(&mqdes->lock, NULL) != 0) {
    free(mqdes);
    return (mqd_t)-1;
  }
  if (pthread_cond_init(&mqdes->cond, NULL) != 0) {
    pthread_mutex_destroy(&mqdes->lock);
    free(mqdes);
    return (mqd_t)-1;
  }
  mqdes->attr = *attr;
  mqdes->attr.mq_curmsgs = 0;
  mqdes->heap = NULL;
  mqdes->heap_length = 0;
  mqdes->heap_size = 0;
  return mqdes;
}
