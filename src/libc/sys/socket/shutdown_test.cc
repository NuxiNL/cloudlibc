// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(shutdown, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad file descriptor.
  ASSERT_EQ(-1, shutdown(-1, SHUT_RDWR));
  ASSERT_EQ(EBADF, errno);

  // Bad shutdown type.
  {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    ASSERT_EQ(-1, shutdown(fds[0], 0xdeadc0de));
    ASSERT_EQ(EINVAL, errno);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }

  // Not a socket.
  {
    ASSERT_EQ(-1, shutdown(fd_tmp, SHUT_RDWR));
    ASSERT_EQ(ENOTSOCK, errno);
  }
}

TEST(shutdown, example) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, fcntl(fds[0], F_SETFL, O_NONBLOCK));

  // Shut down read side.
  char b = 'A';
  ASSERT_EQ(-1, recv(fds[0], &b, 1, 0));
  ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);
  ASSERT_EQ(0, shutdown(fds[0], SHUT_RD));
  ASSERT_EQ(0, recv(fds[0], &b, 1, 0));

  // Shut down write side.
  ASSERT_EQ(1, send(fds[0], &b, 1, 0));
  ASSERT_EQ(0, shutdown(fds[0], SHUT_WR));
  ASSERT_EQ(-1, send(fds[0], &b, 1, 0));
  ASSERT_EQ(EPIPE, errno);

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

static void *sleep_then_shutdown_read(void *arg) {
  // Sleep for a moment to let the recv() settle.
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // Shutdown the given file handle for reading.
  EXPECT_EQ(0, shutdown(*(int *)arg, SHUT_RD));
  return NULL;
}

static void *sleep_then_shutdown_write(void *arg) {
  // Sleep for a moment to let the recv() settle.
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // Shutdown the given file handle for writing.
  EXPECT_EQ(0, shutdown(*(int *)arg, SHUT_WR));
  return NULL;
}

TEST(shutdown, eof) {
  {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

    // Create a thread that will shutdown one end for writing.
    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, NULL, sleep_then_shutdown_write,
                                (void *)&fds[0]));

    // Read from the other end. This should block until the shutdown occurs.
    char buf[2];
    ASSERT_EQ(0, recv(fds[1], buf, sizeof(buf), 0));

    // Clean up.
    ASSERT_EQ(0, pthread_join(thread, NULL));
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }

  {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

    // Create a thread that will shutdown one end for reading.
    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, NULL, sleep_then_shutdown_read,
                                (void *)&fds[0]));

    // Read from the same end. This should block until the shutdown occurs.
    char buf[2];
    ASSERT_EQ(0, recv(fds[0], buf, sizeof(buf), 0));

    // Clean up.
    ASSERT_EQ(0, pthread_join(thread, NULL));
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }
}
