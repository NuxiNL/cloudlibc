// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef MQUEUE_MQUEUE_IMPL_H
#define MQUEUE_MQUEUE_IMPL_H

#include <common/queue.h>

#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// Message.
struct message {
  STAILQ_ENTRY(message) next;  // Next message at the same priority.
  size_t length;               // Length of the message body.
  char contents[];             // Message body.
};

// List of messages, all having the same priority.
struct priority {
  SLIST_ENTRY(priority) next;       // Next priority.
  STAILQ_HEAD(, message) messages;  // List of messages at this priority.
  unsigned int priority;            // Priority of the messages.
};

// Message queue.
struct __mqd {
  pthread_mutex_t lock;               // Queue lock.
  pthread_cond_t cond;                // Queue condition variable for sleeps.
  struct mq_attr attr;                // Queue attributes.
  SLIST_HEAD(, priority) priorities;  // List of priorities having messages.
};

static inline bool mq_receive_pre(mqd_t mqdes, size_t msg_len)
    __trylocks_exclusive(true, mqdes->lock) {
  // Fail if the provided buffer size is less than the message size
  // attribute of the message queue.
  pthread_mutex_lock(&mqdes->lock);
  if (msg_len < (size_t)mqdes->attr.mq_msgsize) {
    pthread_mutex_unlock(&mqdes->lock);
    errno = EMSGSIZE;
    return false;
  }

  // Fail if the queue has no messages and has the non-blocking flag set.
  if (mqdes->attr.mq_curmsgs <= 0 && (mqdes->attr.mq_flags & O_NONBLOCK) != 0) {
    pthread_mutex_unlock(&mqdes->lock);
    errno = EAGAIN;
    return false;
  }
  return true;
}

static inline size_t mq_receive_post(mqd_t mqdes, char *msg_ptr,
                                     unsigned int *msg_prio)
    __unlocks(mqdes->lock) {
  // Determine the highest priority that has a message enqueued.
  struct priority *p = SLIST_FIRST(&mqdes->priorities);
  if (msg_prio != NULL)
    *msg_prio = p->priority;

  // Extract the oldest message from the queue.
  struct message *m = STAILQ_FIRST(&p->messages);
  STAILQ_REMOVE_HEAD(&p->messages, next);
  --mqdes->attr.mq_curmsgs;

  // Remove the priority from the message queue if we no longer have any
  // messages under that priority.
  if (STAILQ_EMPTY(&p->messages)) {
    SLIST_REMOVE_HEAD(&mqdes->priorities, next);
    pthread_mutex_unlock(&mqdes->lock);
    free(p);
  } else {
    pthread_mutex_unlock(&mqdes->lock);
  }

  // Copy out the message contents and free the message.
  size_t length = m->length;
  memcpy(msg_ptr, m->contents, length);
  free(m);
  return length;
}

static inline bool mq_send_pre(mqd_t mqdes, size_t msg_len)
    __trylocks_exclusive(true, mqdes->lock) {
  // Fail if the size of the provided message is more than the message
  // size attribute of the message queue.
  pthread_mutex_lock(&mqdes->lock);
  if (msg_len > (size_t)mqdes->attr.mq_msgsize) {
    pthread_mutex_unlock(&mqdes->lock);
    errno = EMSGSIZE;
    return false;
  }

  // Fail if the queue is full and has the non-blocking flag set.
  if (mqdes->attr.mq_curmsgs >= mqdes->attr.mq_maxmsg &&
      (mqdes->attr.mq_flags & O_NONBLOCK) != 0) {
    pthread_mutex_unlock(&mqdes->lock);
    errno = EAGAIN;
    return false;
  }
  return true;
}

static inline int mq_send_post(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
                               unsigned int msg_prio) __unlocks(mqdes->lock) {
  // Create a message object and initialize it.
  struct message *m = malloc(offsetof(struct message, contents) + msg_len);
  if (m == NULL) {
    pthread_mutex_unlock(&mqdes->lock);
    return -1;
  }
  m->length = msg_len;
  memcpy(m->contents, msg_ptr, msg_len);

  // See if we can already find messages enqueued at the same priority.
  // If so, insert the message into the queue for that priority. As the
  // list of priorities is sorted in descending order, we can stop
  // searching once we reach entries with a lower priority than the new
  // message.
  struct priority *p, *p_prev = NULL;
  SLIST_FOREACH(p, &mqdes->priorities, next) {
    if (p->priority <= msg_prio) {
      if (p->priority == msg_prio) {
        // Found other messages at the same priority.
        STAILQ_INSERT_TAIL(&p->messages, m, next);
        goto inserted;
      }
      break;
    }
    p_prev = p;
  }

  // Allocate a new priority.
  p = malloc(sizeof(*p));
  if (p == NULL) {
    free(m);
    pthread_mutex_unlock(&mqdes->lock);
    return -1;
  }
  STAILQ_INIT(&p->messages);
  STAILQ_INSERT_HEAD(&p->messages, m, next);
  p->priority = msg_prio;
  if (p_prev == NULL) {
    // Message queue is empty or only has messages with a priority lower
    // than that of the new message. Insert the priority at the
    // beginning of the list.
    SLIST_INSERT_HEAD(&mqdes->priorities, p, next);
  } else {
    // Insert priority after the entry with the lowest priority that is
    // higher than that of the new message.
    SLIST_INSERT_AFTER(p_prev, p, next);
  }

inserted:
  // Successfully inserted the message into the queue.
  ++mqdes->attr.mq_curmsgs;
  pthread_mutex_unlock(&mqdes->lock);
  return 0;
}

#endif
