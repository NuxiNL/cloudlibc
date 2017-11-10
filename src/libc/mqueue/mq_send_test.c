// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <testing.h>

TEST(mq_send, bad) {
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = O_NONBLOCK,
      .mq_maxmsg = 1,
      .mq_msgsize = 10,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));

  // Message is too large to fit in the queue.
  ASSERT_EQ(-1, mq_send(mqd, "Hello, world", 12, 123));
  ASSERT_EQ(EMSGSIZE, errno);

  // Sending too many messages should fail.
  ASSERT_EQ(0, mq_send(mqd, "Hello", 5, 123));
  ASSERT_EQ(-1, mq_send(mqd, "Hello", 5, 123));
  ASSERT_EQ(EAGAIN, errno);

  ASSERT_EQ(0, mq_destroy(mqd));
}

static void *pull_message(void *argument) {
  // Pull the first message from the message queue after a slight delay.
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0,
                               &(struct timespec){.tv_nsec = 250000000}));
  char buf[5];
  unsigned int prio;
  ASSERT_EQ(5, mq_receive(*(mqd_t *)argument, buf, sizeof(buf), &prio));
  ASSERT_ARREQ("Hello", buf, sizeof(buf));
  ASSERT_EQ(123, prio);
  return NULL;
}

TEST(mq_send, blocking) {
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = 0,
      .mq_maxmsg = 1,
      .mq_msgsize = 5,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));
  ASSERT_EQ(0, mq_send(mqd, "Hello", 5, 123));

  // Spawn a background thread to empty the queue with a little delay.
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, pull_message, &mqd));

  // Block on the message queue while sending a message.
  ASSERT_EQ(0, mq_send(mqd, "World", 5, 456));

  // The second message should be immediately readable.
  char buf[5];
  unsigned int prio;
  ASSERT_EQ(5, mq_receive(mqd, buf, sizeof(buf), &prio));
  ASSERT_ARREQ("World", buf, sizeof(buf));
  ASSERT_EQ(456, prio);

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, mq_destroy(mqd));
}
