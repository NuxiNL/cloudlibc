// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <testing.h>

TEST(mq_timedsend, bad) {
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = O_NONBLOCK, .mq_maxmsg = 1, .mq_msgsize = 10,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));

  // Message is too large to fit in the queue.
  ASSERT_EQ(-1,
            mq_timedsend(mqd, "Hello, world", 12, 123, &(struct timespec){}));
  ASSERT_EQ(EMSGSIZE, errno);

  // Fill up the queue for the tests below. The abstime value is allowed
  // to be illegal in case we don't block.
  ASSERT_EQ(
      0, mq_timedsend(mqd, "Hello", 5, 123, &(struct timespec){.tv_nsec = -1}));

  // The queue should now be full, meaning we have to return EAGAIN.
  ASSERT_EQ(-1, mq_timedsend(mqd, "Hello", 5, 123, &(struct timespec){}));
  ASSERT_EQ(EAGAIN, errno);

  // If we turn of non-blocking mode, we should get ETIMEDOUT instead.
  attr.mq_flags = 0;
  ASSERT_EQ(0, mq_setattr(mqd, &attr, NULL));
  ASSERT_EQ(-1, mq_timedsend(mqd, "Hello", 5, 123, &(struct timespec){}));
  ASSERT_EQ(ETIMEDOUT, errno);

  // Invalid value for abstime.
  ASSERT_EQ(-1, mq_timedsend(mqd, "Hello", 5, 123,
                             &(struct timespec){.tv_nsec = -1}));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(-1, mq_timedsend(mqd, "Hello", 5, 123,
                             &(struct timespec){.tv_nsec = 1000000000}));
  ASSERT_EQ(EINVAL, errno);

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

TEST(mq_timedsend, blocking) {
  // Initialize a queue that is already full. As we won't block, it
  // should be allowed to provide an invalid abstime.
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = 0, .mq_maxmsg = 1, .mq_msgsize = 5,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));
  ASSERT_EQ(
      0, mq_timedsend(mqd, "Hello", 5, 123, &(struct timespec){.tv_nsec = -1}));

  // Spawn a background thread to empty the queue with a little delay.
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, pull_message, &mqd));

  // Block on the message queue while sending a message.
  struct timespec abstime;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &abstime));
  ++abstime.tv_sec;
  ASSERT_EQ(0, mq_timedsend(mqd, "World", 5, 456, &abstime));

  // The second message should be immediately readable.
  char buf[5];
  unsigned int prio;
  ASSERT_EQ(5, mq_receive(mqd, buf, sizeof(buf), &prio));
  ASSERT_ARREQ("World", buf, sizeof(buf));
  ASSERT_EQ(456, prio);

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, mq_destroy(mqd));
}
