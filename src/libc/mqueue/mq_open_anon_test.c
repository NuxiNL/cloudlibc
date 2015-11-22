// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <testing.h>

TEST(mq_open_anon, bad) {
  // Bad mq_flags. Only O_NONBLOCK can be passed in.
  {
    struct mq_attr attr = {
        .mq_flags = O_RDWR | O_CREAT | O_EXCL, .mq_maxmsg = 1, .mq_msgsize = 1,
    };
    ASSERT_EQ((mqd_t)-1, mq_open_anon(&attr));
    ASSERT_EQ(EINVAL, errno);
  }

  // Bad maximum number of messages.
  {
    struct mq_attr attr = {
        .mq_flags = 0, .mq_maxmsg = 0, .mq_msgsize = 1,
    };
    ASSERT_EQ((mqd_t)-1, mq_open_anon(&attr));
    ASSERT_EQ(EINVAL, errno);
  }

  // Bad maximum message size.
  {
    struct mq_attr attr = {
        .mq_flags = 0, .mq_maxmsg = 100, .mq_msgsize = -5,
    };
    ASSERT_EQ((mqd_t)-1, mq_open_anon(&attr));
    ASSERT_EQ(EINVAL, errno);
  }
}

TEST(mq_open_anon, example) {
  // Open blocking queue.
  {
    struct mq_attr attr = {
        .mq_flags = 0, .mq_maxmsg = 25, .mq_msgsize = 4096,
    };
    mqd_t mq = mq_open_anon(&attr);
    ASSERT_NE((mqd_t)-1, mq);
    ASSERT_EQ(0, mq_close(mq));
  }

  // Open non-blocking queue.
  {
    struct mq_attr attr = {
        .mq_flags = O_NONBLOCK, .mq_maxmsg = 100, .mq_msgsize = 32,
    };
    mqd_t mq = mq_open_anon(&attr);
    ASSERT_NE((mqd_t)-1, mq);
    ASSERT_EQ(0, mq_close(mq));
  }
}
