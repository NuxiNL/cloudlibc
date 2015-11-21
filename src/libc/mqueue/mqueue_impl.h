// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef MQUEUE_MQUEUE_IMPL_H
#define MQUEUE_MQUEUE_IMPL_H

#include <common/queue.h>

#include <sys/types.h>

#include <errno.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdbool.h>

// Message.
struct message {
  STAILQ_ENTRY(message) next;  // Next message at the same priority.
  size_t length;               // Length of the message body.
  char contents[];             // Message body.
};

// List of messages, all having the same priority.
struct priority {
  unsigned int priority;            // Priority of the messages.
  STAILQ_HEAD(, message) messages;  // List of messages.
};

// Message queue.
struct __mqd {
  pthread_mutex_t lock;   // Queue lock.
  pthread_cond_t cond;    // Queue condition variable for sleeps.
  struct mq_attr attr;    // Queue attributes.
  struct priority *heap;  // Binary heap of message priorities.
  size_t heap_length;     // Number of priorities.
  size_t heap_size;       // Binary heap capacity.
};

static inline bool mq_receive_pre(mqd_t mqdes)
    __trylocks_exclusive(true, mqdes->lock) {
  // TODO(ed): Implement.
  errno = ENOSYS;
  return false;
}

static inline ssize_t mq_receive_post(mqd_t mqdes, const char *msg_ptr,
                                      size_t msg_len, unsigned int *msg_prio)
    __unlocks(mqdes->lock) {
  // TODO(ed): Implement.
  pthread_mutex_unlock(&mqdes->lock);
  errno = ENOSYS;
  return -1;
}

static inline bool mq_send_pre(mqd_t mqdes, size_t msg_len)
    __trylocks_exclusive(true, mqdes->lock) {
  // TODO(ed): Implement.
  errno = ENOSYS;
  return false;
}

static inline int mq_send_post(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
                               unsigned int msg_prio) __unlocks(mqdes->lock) {
  // TODO(ed): Implement.
  pthread_mutex_unlock(&mqdes->lock);
  errno = ENOSYS;
  return -1;
}

#endif
