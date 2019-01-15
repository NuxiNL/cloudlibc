// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <mqueue.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(mq_sendreceive, ordering) {
  // Create a large number of one-byte random messages.
  char messages[4096];
  arc4random_buf(messages, sizeof(messages));

  // Create a message queue that will hold these messages.
  mqd_t mqd;
  {
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = sizeof(messages),
        .mq_msgsize = 1,
    };
    ASSERT_EQ(0, mq_init(&mqd, &attr));
  }

  for (int i = 0; i < 100; ++i) {
    // Divide the messages across a number of priorities.
    const size_t nqueues = 32;
    const size_t queuelen = sizeof(messages) / nqueues;

    // Insert the messages into the queue sequentially, but alternate
    // between the priorities randomly.
    int offsets[nqueues] = {};
    for (size_t j = 1; j <= sizeof(messages); ++j) {
      unsigned int queue;
      do {
        queue = arc4random_uniform(nqueues);
      } while (offsets[queue] == queuelen);
      mq_send(mqd, &messages[queue * queuelen + offsets[queue]++], 1,
              nqueues - queue);

      struct mq_attr attr;
      ASSERT_EQ(0, mq_getattr(mqd, &attr));
      ASSERT_EQ(0, attr.mq_flags);
      ASSERT_EQ(sizeof(messages), attr.mq_maxmsg);
      ASSERT_EQ(1, attr.mq_msgsize);
      ASSERT_EQ(j, attr.mq_curmsgs);
    }

    // Extract messages out of the queue one by one. The ordering in
    // which they are returned should be identical to the layout of the
    // originally created messages array.
    for (size_t j = 0; j < sizeof(messages); ++j) {
      char c;
      unsigned int prio;
      ASSERT_EQ(1, mq_receive(mqd, &c, 1, &prio));
      ASSERT_EQ(messages[j], c);
      ASSERT_EQ(nqueues - j / queuelen, prio);

      struct mq_attr attr;
      ASSERT_EQ(0, mq_getattr(mqd, &attr));
      ASSERT_EQ(0, attr.mq_flags);
      ASSERT_EQ(sizeof(messages), attr.mq_maxmsg);
      ASSERT_EQ(1, attr.mq_msgsize);
      ASSERT_EQ(sizeof(messages) - j - 1, attr.mq_curmsgs);
    }
  }

  ASSERT_EQ(0, mq_destroy(mqd));
}
