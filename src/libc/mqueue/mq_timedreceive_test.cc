// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(mq_timedreceive, bad) {
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = O_NONBLOCK,
      .mq_maxmsg = 10,
      .mq_msgsize = 10,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));

  // Buffer is too small to fix the largest message.
  char buf[10];
  {
    struct timespec ts = {};
    ASSERT_EQ(-1, mq_timedreceive(mqd, buf, sizeof(buf) - 1, NULL, &ts));
  }
  ASSERT_EQ(EMSGSIZE, errno);

  // Queue is empty and non-blocking mode is enabled.
  {
    struct timespec ts = {};
    ASSERT_EQ(-1, mq_timedreceive(mqd, buf, sizeof(buf), NULL, &ts));
  }
  ASSERT_EQ(EAGAIN, errno);

  // If we turn of non-blocking mode, we should get ETIMEDOUT instead.
  attr.mq_flags = 0;
  ASSERT_EQ(0, mq_setattr(mqd, &attr, NULL));
  {
    struct timespec ts = {};
    ASSERT_EQ(-1, mq_timedreceive(mqd, buf, sizeof(buf), NULL, &ts));
  }
  ASSERT_EQ(ETIMEDOUT, errno);

  // Invalid value for abstime.
  {
    struct timespec ts = {.tv_nsec = -1};
    ASSERT_EQ(-1, mq_timedreceive(mqd, buf, sizeof(buf), NULL, &ts));
  }
  ASSERT_EQ(EINVAL, errno);
  {
    struct timespec ts = {.tv_nsec = 1000000000};
    ASSERT_EQ(-1, mq_timedreceive(mqd, buf, sizeof(buf), NULL, &ts));
  }
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(0, mq_destroy(mqd));
}

static void *push_message(void *argument) {
  // Push a message into the message queue after a slight delay.
  struct timespec ts = {.tv_nsec = 250000000};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  EXPECT_EQ(0, mq_send(*(mqd_t *)argument, "Hello", 5, 123));
  return NULL;
}

TEST(mq_timedreceive, blocking) {
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = 0,
      .mq_maxmsg = 1,
      .mq_msgsize = 5,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));

  // Spawn a background thread to send a message with a little delay.
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, push_message, &mqd));

  // Block on the message queue while receiving a message.
  struct timespec abstime;
  ASSERT_EQ(0, clock_gettime(CLOCK_REALTIME, &abstime));
  ++abstime.tv_sec;
  char buf[6];
  unsigned int prio;
  ASSERT_EQ(5, mq_timedreceive(mqd, buf, sizeof(buf), &prio, &abstime));
  buf[5] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello"));
  ASSERT_EQ(123, prio);

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, mq_destroy(mqd));
}
