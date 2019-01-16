// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/select.h>

#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(select, ebadf) {
  // Create a bad file descriptor number.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // If the 'nfds' argument of select() doesn't exceed our bad file
  // descriptor number, polling should be unaffected.
  {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fds[0], &readfds);
    struct timeval timeout = {};
    ASSERT_EQ(0, select(fds[0], &readfds, NULL, NULL, &timeout));
  }

  // In case it does, we should trigger EBADF.
  {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fds[0], &readfds);
    ASSERT_EQ(-1, select(fds[0] + 1, &readfds, NULL, NULL, NULL));
    ASSERT_EQ(EBADF, errno);
  }
}

TEST(select, einval) {
  // Negative file descriptor upper bound.
  ASSERT_EQ(-1, select(-1, NULL, NULL, NULL, NULL));
  ASSERT_EQ(EINVAL, errno);

  // Bad timeout value.
  {
    struct timeval timeout = {.tv_usec = -1};
    ASSERT_EQ(-1, select(0, NULL, NULL, NULL, &timeout));
    ASSERT_EQ(EINVAL, errno);
  }
  {
    struct timeval timeout = {.tv_usec = 1000000};
    ASSERT_EQ(-1, select(0, NULL, NULL, NULL, &timeout));
    ASSERT_EQ(EINVAL, errno);
  }
}

TEST(select, enosys) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Don't allow a non-empty errorfds set. This implementation does not
  // support polling for out-of-band data.
  {
    struct timeval timeout = {};
    ASSERT_EQ(0, select(0, NULL, NULL, NULL, &timeout));
  }
  {
    fd_set errorfds;
    FD_ZERO(&errorfds);
    struct timeval timeout = {};
    ASSERT_EQ(0, select(0, NULL, NULL, &errorfds, &timeout));
  }
  {
    fd_set errorfds;
    FD_ZERO(&errorfds);
    FD_SET(fd_tmp, &errorfds);
    struct timeval timeout = {};
    ASSERT_EQ(-1, select(0, NULL, NULL, &errorfds, &timeout));
    ASSERT_EQ(ENOSYS, errno);
  }
}

TEST(select, pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  int nfds = (fds[0] > fds[1] ? fds[0] : fds[1]) + 1;

  // Poll an empty pipe. Data can be written into it.
  {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fds[0], &readfds);
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fds[1], &writefds);
    ASSERT_EQ(1, select(nfds, &readfds, &writefds, NULL, NULL));
    ASSERT_FALSE(FD_ISSET(fds[0], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[1], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[0], &writefds));
    ASSERT_TRUE(FD_ISSET(fds[1], &writefds));
  }

  // Perform a non-blocking poll.
  {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fds[0], &readfds);
    struct timeval timeout = {};
    ASSERT_EQ(0, select(nfds, &readfds, NULL, NULL, &timeout));
    ASSERT_FALSE(FD_ISSET(fds[0], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[1], &readfds));
  }

  // Write some data into it. We can now read from the pipe.
  {
    ASSERT_EQ(3, write(fds[1], "Hi\n", 3));
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fds[0], &readfds);
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fds[1], &writefds);
    ASSERT_EQ(2, select(nfds, &readfds, &writefds, NULL, NULL));
    ASSERT_TRUE(FD_ISSET(fds[0], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[1], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[0], &writefds));
    ASSERT_TRUE(FD_ISSET(fds[1], &writefds));
  }

  // Fill the pipe up with data. We can no longer write.
  {
    ASSERT_EQ(0, fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK));
    char buf[1024] = {};
    while (write(fds[1], buf, sizeof(buf)) > 0) {
    }
    ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fds[0], &readfds);
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fds[1], &writefds);
    ASSERT_EQ(1, select(nfds, &readfds, &writefds, NULL, NULL));
    ASSERT_TRUE(FD_ISSET(fds[0], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[1], &readfds));
    ASSERT_FALSE(FD_ISSET(fds[0], &writefds));
    ASSERT_FALSE(FD_ISSET(fds[1], &writefds));
  }

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(select, sleep) {
  // We should sleep at least 1 second.
  time_t before = time(NULL);
  struct timeval timeout = {.tv_sec = 1, .tv_usec = 500000};
  ASSERT_EQ(0, select(0, NULL, NULL, NULL, &timeout));
  time_t after = time(NULL);
  ASSERT_LE(before + 1, after);
}
