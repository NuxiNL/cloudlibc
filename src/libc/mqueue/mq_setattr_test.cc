// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(mq_setattr, bad) {
  mqd_t mqd;
  struct mq_attr attr = {
      .mq_flags = 0,
      .mq_maxmsg = 100,
      .mq_msgsize = 100,
  };
  ASSERT_EQ(0, mq_init(&mqd, &attr));

  attr.mq_flags = 0xdeadc0de;
  ASSERT_EQ(-1, mq_setattr(mqd, &attr, NULL));
  ASSERT_EQ(EINVAL, errno);

  ASSERT_EQ(0, mq_destroy(mqd));
}

TEST(mq_setattr, example) {
  mqd_t mqd;
  {
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 100,
        .mq_msgsize = 100,
    };
    ASSERT_EQ(0, mq_init(&mqd, &attr));
  }

  // Enable non-blocking mode. All other attributes should be ignored.
  {
    struct mq_attr attr = {
        .mq_flags = O_NONBLOCK,
        .mq_maxmsg = 12345,
        .mq_msgsize = 23456,
        .mq_curmsgs = 34567,
    };
    ASSERT_EQ(0, mq_setattr(mqd, &attr, NULL));
    ASSERT_EQ(0, mq_getattr(mqd, &attr));
    ASSERT_EQ(O_NONBLOCK, attr.mq_flags);
    ASSERT_EQ(100, attr.mq_maxmsg);
    ASSERT_EQ(100, attr.mq_msgsize);
    ASSERT_EQ(0, attr.mq_curmsgs);
  }

  // Disable non-blocking mode again. Fetch old attributes while setting.
  {
    struct mq_attr attr = {
        .mq_flags = 0,
    };
    ASSERT_EQ(0, mq_setattr(mqd, &attr, &attr));
    ASSERT_EQ(O_NONBLOCK, attr.mq_flags);
    ASSERT_EQ(100, attr.mq_maxmsg);
    ASSERT_EQ(100, attr.mq_msgsize);
    ASSERT_EQ(0, attr.mq_curmsgs);
  }

  ASSERT_EQ(0, mq_destroy(mqd));
}
