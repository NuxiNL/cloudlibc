// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(mq_init, bad) {
  // Bad mq_flags. Only O_NONBLOCK can be passed in.
  {
    struct mq_attr attr = {
        .mq_flags = O_RDWR | O_CREAT | O_EXCL,
        .mq_maxmsg = 1,
        .mq_msgsize = 1,
    };
    ASSERT_EQ(-1, mq_init(NULL, &attr));
    ASSERT_EQ(EINVAL, errno);
  }

  // Bad maximum number of messages.
  {
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 0,
        .mq_msgsize = 1,
    };
    ASSERT_EQ(-1, mq_init(NULL, &attr));
    ASSERT_EQ(EINVAL, errno);
  }

  // Bad maximum message size.
  {
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 100,
        .mq_msgsize = -5,
    };
    ASSERT_EQ(-1, mq_init(NULL, &attr));
    ASSERT_EQ(EINVAL, errno);
  }
}

TEST(mq_init, example) {
  // Open blocking queue.
  {
    mqd_t mqd;
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 25,
        .mq_msgsize = 4096,
    };
    ASSERT_EQ(0, mq_init(&mqd, &attr));
    ASSERT_EQ(0, mq_destroy(mqd));
  }

  // Open non-blocking queue.
  {
    mqd_t mqd;
    struct mq_attr attr = {
        .mq_flags = O_NONBLOCK,
        .mq_maxmsg = 100,
        .mq_msgsize = 32,
    };
    ASSERT_EQ(0, mq_init(&mqd, &attr));
    ASSERT_EQ(0, mq_destroy(mqd));
  }
}
