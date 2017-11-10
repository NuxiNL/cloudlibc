// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef MQUEUE_MQUEUE_IMPL_H
#define MQUEUE_MQUEUE_IMPL_H

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
//
// Messages are stored in a linked list, sorted in the order in which
// they should be extracted by mq_receive(). As mq_send() has to use the
// message priority to determine where the message needs to be inserted,
// we make use of a skip list.
//
// The {queue,next}_receive pointers point to every individual message,
// whereas the {queue_next}_send pointers only point to the last message
// at every priority. This allows us to insert messages in O(p) time,
// where p is the number of different message priorities used.
//
// In most common setups, p tends to be pretty low.
struct message {
  struct message *next_receive;  // Next message to be returned.
  struct message *next_send;     // Last message of the next priority.
  size_t length;                 // Length of the message body.
  unsigned int priority;         // Priority of the message.
  char contents[];               // Message body.
};

// Message queue.
struct __mqd {
  pthread_mutex_t lock;           // Queue lock.
  pthread_cond_t cond_receive;    // Queue condition variable for receive.
  pthread_cond_t cond_send;       // Queue condition variable for send.
  struct mq_attr attr;            // Queue attributes.
  struct message *queue_receive;  // List of messages to be returned.
  struct message *queue_send;     // Last message of the highest priority.
};

static inline bool mq_receive_pre(struct __mqd *mqd, size_t msg_len)
    __trylocks_exclusive(true, mqd->lock) {
  // Fail if the provided buffer size is less than the message size
  // attribute of the message queue.
  pthread_mutex_lock(&mqd->lock);
  if (msg_len < (size_t)mqd->attr.mq_msgsize) {
    pthread_mutex_unlock(&mqd->lock);
    errno = EMSGSIZE;
    return false;
  }

  // Fail if the queue has no messages and has the non-blocking flag set.
  if (mqd->attr.mq_curmsgs <= 0 && (mqd->attr.mq_flags & O_NONBLOCK) != 0) {
    pthread_mutex_unlock(&mqd->lock);
    errno = EAGAIN;
    return false;
  }
  return true;
}

static inline size_t mq_receive_post(struct __mqd *mqd, char *msg_ptr,
                                     unsigned int *msg_prio)
    __unlocks(mqd->lock) {
  // Extract the oldest message from the queue.
  struct message *m = mqd->queue_receive;
  mqd->queue_receive = m->next_receive;
  --mqd->attr.mq_curmsgs;

  // If the message is the only message at that priority, update the
  // skip list to point to the next priority.
  if (mqd->queue_send == m)
    mqd->queue_send = m->next_send;
  pthread_mutex_unlock(&mqd->lock);
  pthread_cond_signal(&mqd->cond_send);

  // Copy out the message contents and free it.
  size_t length = m->length;
  memcpy(msg_ptr, m->contents, length);
  if (msg_prio != NULL)
    *msg_prio = m->priority;
  free(m);
  return length;
}

static inline bool mq_send_pre(struct __mqd *mqd, size_t msg_len)
    __trylocks_exclusive(true, mqd->lock) {
  // Fail if the size of the provided message is more than the message
  // size attribute of the message queue.
  pthread_mutex_lock(&mqd->lock);
  if (msg_len > (size_t)mqd->attr.mq_msgsize) {
    pthread_mutex_unlock(&mqd->lock);
    errno = EMSGSIZE;
    return false;
  }

  // Fail if the queue is full and has the non-blocking flag set.
  if (mqd->attr.mq_curmsgs >= mqd->attr.mq_maxmsg &&
      (mqd->attr.mq_flags & O_NONBLOCK) != 0) {
    pthread_mutex_unlock(&mqd->lock);
    errno = EAGAIN;
    return false;
  }
  return true;
}

static inline int mq_send_post(struct __mqd *mqd, const char *msg_ptr,
                               size_t msg_len, unsigned int msg_prio)
    __unlocks(mqd->lock) {
  // Create a message object and initialize it.
  struct message *m = malloc(offsetof(struct message, contents) + msg_len);
  if (m == NULL) {
    pthread_mutex_unlock(&mqd->lock);
    return -1;
  }
  m->length = msg_len;
  m->priority = msg_prio;
  memcpy(m->contents, msg_ptr, msg_len);

  // Scan through the list of messages to find the spot where the
  // message needs to be inserted.
  struct message **m_receive = &mqd->queue_receive;
  struct message **m_send = &mqd->queue_send;
  while (*m_send != NULL) {
    if ((*m_send)->priority <= msg_prio) {
      if ((*m_send)->priority == msg_prio) {
        // We already have other messages enqueued at the same priority.
        // Place the message behind the currently last message at that
        // priority and update the skip list to point to the new message
        // instead.
        m->next_receive = (*m_send)->next_receive;
        (*m_send)->next_receive = m;
        m->next_send = (*m_send)->next_send;
        *m_send = m;
        goto inserted;
      }
      break;
    }
    m_receive = &(*m_send)->next_receive;
    m_send = &(*m_send)->next_send;
  }

  // First message at this priority. Insert the message into both the
  // receive and send queue.
  m->next_receive = *m_receive;
  *m_receive = m;
  m->next_send = *m_send;
  *m_send = m;

inserted:
  // Successfully inserted the message into the queue.
  ++mqd->attr.mq_curmsgs;
  pthread_mutex_unlock(&mqd->lock);
  pthread_cond_signal(&mqd->cond_receive);
  return 0;
}

#endif
