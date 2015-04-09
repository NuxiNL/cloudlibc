// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <testing.h>
#include <unistd.h>

TEST(poll, pipe) {
  // Poll an empty pipe. Data can be written into it.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  {
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
        {.fd = -123, .events = POLLRDNORM | POLLWRNORM},
    };
    ASSERT_EQ(1, poll(pfds, __arraycount(pfds), -1));
    ASSERT_EQ(0, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
    ASSERT_EQ(0, pfds[2].revents);
  }

  // Perform a non-blocking poll.
  {
    struct pollfd pfd;
    ASSERT_EQ(0, poll(&pfd, 1, 0));
    ASSERT_EQ(0, pfd.revents);
  }

  // Write some data into it. We can now read from the pipe.
  {
    ASSERT_EQ(3, write(fds[1], "Hi\n", 3));
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
        {.fd = fds[1], .events = 0},
    };
    ASSERT_EQ(2, poll(pfds, __arraycount(pfds), -1));
    ASSERT_EQ(POLLRDNORM, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
    ASSERT_EQ(0, pfds[2].revents);
  }

  // Fill the pipe up with data. We can no longer write.
  {
    ASSERT_EQ(0, fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK));
    char buf[1024] = {};
    while (write(fds[1], buf, sizeof(buf)) > 0) {
    }
    ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
    };
    ASSERT_EQ(1, poll(pfds, __arraycount(pfds), -1));
    ASSERT_EQ(POLLRDNORM, pfds[0].revents);
    ASSERT_EQ(0, pfds[1].revents);
  }

  // TODO(edje): Test POLLHUP.

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

// TODO(edje): Test sockets, regular files, etc.

TEST_SEPARATE_PROCESS(poll, pollnval) {
  // Create a bad file descriptor number.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));

  struct pollfd pfd = {.fd = fds[0], POLLRDNORM | POLLWRNORM};
  ASSERT_EQ(1, poll(&pfd, 1, -1));
  ASSERT_EQ(POLLNVAL, pfd.revents);
}
