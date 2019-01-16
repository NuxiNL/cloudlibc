// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>
#include <iterator>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(poll, enosys) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // For now, not setting POLLRDNORM/POLLWRNORM is not supported. Each
  // entry is decomposed into separate read/write polls, meaning that if
  // none of these flags are provided, we would simply get stuck
  // indefinitely.
  struct pollfd pfd = {.fd = fd_tmp, .events = 0};
  ASSERT_EQ(-1, poll(&pfd, 1, -1));
  ASSERT_EQ(ENOSYS, errno);
}

TEST(poll, pipe) {
  // Poll an empty pipe. Data can be written into it.
  int fds[2];
  {
    ASSERT_EQ(0, pipe(fds));
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
        {.fd = -123, .events = POLLRDNORM | POLLWRNORM},
    };
    ASSERT_EQ(1, poll(pfds, std::size(pfds), -1));
    ASSERT_EQ(0, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
    ASSERT_EQ(0, pfds[2].revents);
  }

  // Perform a non-blocking poll.
  {
    struct pollfd pfd = {.fd = fds[0], .events = POLLRDNORM};
    ASSERT_EQ(0, poll(&pfd, 1, 0));
    ASSERT_EQ(0, pfd.revents);
  }

  // Write some data into it. We can now read from the pipe.
  {
    ASSERT_EQ(3, write(fds[1], "Hi\n", 3));
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLWRNORM},
        {.fd = -123, .events = POLLRDNORM},
    };
    ASSERT_EQ(2, poll(pfds, std::size(pfds), -1));
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
    ASSERT_EQ(1, poll(pfds, std::size(pfds), -1));
    ASSERT_EQ(POLLRDNORM, pfds[0].revents);
    ASSERT_EQ(0, pfds[1].revents);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }

  // Close the write side of the pipe. We should still observe both
  // POLLRDNORM and POLLHUP.
  {
    ASSERT_EQ(0, pipe(fds));
    struct pollfd pfd = {.fd = fds[0], .events = POLLRDNORM};
    ASSERT_EQ(0, poll(&pfd, 1, 0));
    ASSERT_EQ(0, close(fds[1]));
    ASSERT_EQ(1, poll(&pfd, 1, -1));
    ASSERT_EQ(POLLRDNORM | POLLHUP, pfd.revents);
    ASSERT_EQ(0, close(fds[0]));
  }

  // When closing the read side of a pipe, we should only observe
  // POLLHUP on the write side -- not POLLWRNORM.
  {
    ASSERT_EQ(0, pipe(fds));
    struct pollfd pfd = {.fd = fds[1], .events = POLLWRNORM};
    ASSERT_EQ(1, poll(&pfd, 1, 0));
    ASSERT_EQ(POLLWRNORM, pfd.revents);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(1, poll(&pfd, 1, -1));
    ASSERT_EQ(POLLHUP, pfd.revents);
    ASSERT_EQ(0, close(fds[1]));
  }
}

TEST(poll, socket) {
  // Poll an empty socket pair.
  int fds[2];
  {
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM | POLLWRNORM},
        {.fd = fds[1], .events = POLLRDNORM | POLLWRNORM},
    };
    ASSERT_EQ(2, poll(pfds, std::size(pfds), -1));
    ASSERT_EQ(POLLWRNORM, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
  }

  // Perform a non-blocking poll.
  {
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM},
        {.fd = fds[1], .events = POLLRDNORM},
    };
    ASSERT_EQ(0, poll(pfds, std::size(pfds), 0));
    ASSERT_EQ(0, pfds[0].revents);
    ASSERT_EQ(0, pfds[1].revents);
  }

  // Write some data into it. We can now read from the socket.
  {
    ASSERT_EQ(3, write(fds[1], "Hi\n", 3));
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM | POLLWRNORM},
        {.fd = fds[1], .events = POLLRDNORM | POLLWRNORM},
    };
    ASSERT_EQ(2, poll(pfds, std::size(pfds), -1));
    ASSERT_EQ(POLLRDNORM | POLLWRNORM, pfds[0].revents);
    ASSERT_EQ(POLLWRNORM, pfds[1].revents);
  }

  // Fill the socket up with data. We can no longer write.
  {
    ASSERT_EQ(0, fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK));
    char buf[1024] = {};
    while (write(fds[1], buf, sizeof(buf)) > 0) {
    }
    ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);
    struct pollfd pfds[] = {
        {.fd = fds[0], .events = POLLRDNORM | POLLWRNORM},
        {.fd = fds[1], .events = POLLRDNORM | POLLWRNORM},
    };
    ASSERT_EQ(1, poll(pfds, std::size(pfds), -1));
    ASSERT_EQ(POLLRDNORM | POLLWRNORM, pfds[0].revents);
    ASSERT_EQ(0, pfds[1].revents);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }

  // Close one side of the socket. We should observe POLLRDNORM in
  // combination with POLLHUP. POLLWRNORM should no longer be set.
  {
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    ASSERT_EQ(0, close(fds[1]));
    struct pollfd pfd = {.fd = fds[0], .events = POLLRDNORM | POLLWRNORM};
    ASSERT_EQ(1, poll(&pfd, 1, -1));
    ASSERT_EQ(POLLRDNORM | POLLHUP, pfd.revents);
    ASSERT_EQ(0, close(fds[0]));
  }
}

TEST(poll, file) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Regular file should always trigger read and write.
  int fd = openat(fd_tmp, "hello", O_RDWR | O_CREAT);
  ASSERT_LE(0, fd);
  struct pollfd pfd = {.fd = fd, .events = POLLRDNORM | POLLWRNORM};
  ASSERT_EQ(1, poll(&pfd, 1, -1));
  ASSERT_EQ(POLLRDNORM | POLLWRNORM, pfd.revents);
  ASSERT_EQ(0, close(fd));
}

TEST(poll, pollnval) {
  // Create a bad file descriptor number.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));

  struct pollfd pfd = {.fd = fds[0], POLLRDNORM | POLLWRNORM};
  ASSERT_EQ(1, poll(&pfd, 1, -1));
  ASSERT_EQ(POLLNVAL, pfd.revents);

  ASSERT_EQ(0, close(fds[1]));
}

TEST(poll, sleep) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  ASSERT_EQ(0, poll(NULL, 0, 1500));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
