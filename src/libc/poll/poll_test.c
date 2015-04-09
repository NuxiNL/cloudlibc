// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <poll.h>
#include <testing.h>
#include <unistd.h>

TEST(poll, pipe) {
  // Poll an empty pipe. Data can be written into it.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  {
    struct pollfd pfds[2] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
    };
    ASSERT_EQ(1, poll(pfds, __arraycount(pfds), -1));
    ASSERT_EQ(0, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
  }

  // Write some data into it. We can now read from the pipe.
  ASSERT_EQ(3, write(fds[1], "Hi\n", 3));
  {
    struct pollfd pfds[2] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
    };
    ASSERT_EQ(2, poll(pfds, __arraycount(pfds), -1));
    ASSERT_EQ(POLLRDNORM, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
  }

  // TODO(edje): Add more tests.

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
