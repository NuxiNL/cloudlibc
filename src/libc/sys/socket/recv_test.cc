// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(recv, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad file descriptor.
  char b;
  ASSERT_EQ(-1, recv(-1, &b, 1, 0));
  ASSERT_EQ(EBADF, errno);

  // Not a socket.
  {
    ASSERT_EQ(-1, recv(fd_tmp, &b, 1, 0));
    ASSERT_EQ(ENOTSOCK, errno);
  }

  {
    // Blocking recv() on non-blocking file descriptor.
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    ASSERT_EQ(0, fcntl(fds[0], F_SETFL, O_NONBLOCK));
    ASSERT_EQ(-1, recv(fds[0], &b, 1, 0));
    ASSERT_TRUE(errno == EAGAIN || errno == EWOULDBLOCK);

    // Bad flags.
    ASSERT_EQ(-1, recv(fds[0], &b, 1, 0xdeadc0de));
    ASSERT_EQ(EOPNOTSUPP, errno);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }
}

TEST(recv, example) {
  // Create socket pair.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  // Send a message into one socket.
  ASSERT_EQ(6, write(fds[0], "Hello ", 6));
  ASSERT_EQ(6, write(fds[0], "world!", 6));

  // Take a peek at the contents in the socket buffer.
  {
    char buf[11];
    ASSERT_EQ(10, recv(fds[1], buf, sizeof(buf) - 1, MSG_PEEK));
    buf[10] = '\0';
    ASSERT_STREQ("Hello worl", buf);
  }

  // Receive it back through the other socket.
  {
    char buf[13];
    ASSERT_EQ(12, recv(fds[1], buf, sizeof(buf) - 1, 0));
    buf[12] = '\0';
    ASSERT_STREQ("Hello world!", buf);
  }

  // Destroy socketpair.
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

static void *do_send(void *arg) {
  // Send two pieces of data with a small delay in between.
  int fd = (intptr_t)arg;
  EXPECT_EQ(6, send(fd, "Hello ", 6, 0));
  struct timespec ts = {.tv_nsec = 100000000L};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
  EXPECT_EQ(6, send(fd, "world!", 6, 0));
  return NULL;
}

TEST(recv, waitall) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  pthread_t thread;
  ASSERT_EQ(0,
            pthread_create(&thread, NULL, do_send, (void *)(intptr_t)fds[1]));

  // The recv() call should block until both send() calls in do_send()
  // have been completed, as MSG_WAITALL has been passed in.
  char buf[13];
  ASSERT_EQ(sizeof(buf) - 1, recv(fds[0], buf, sizeof(buf) - 1, MSG_WAITALL));
  buf[12] = '\0';
  ASSERT_STREQ("Hello world!", buf);

  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
