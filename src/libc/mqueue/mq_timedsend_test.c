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

  // The queue should now be full, meaning we ahve to return EAGAIN.
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

// TODO(ed): Add more tests.
