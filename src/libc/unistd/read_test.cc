// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(read, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad file descriptor.
  char b;
  ASSERT_EQ(-1, read(0xdeadc0de, &b, 1));
  ASSERT_EQ(EBADF, errno);

  // Invalid file descriptor type.
  int fd = openat(fd_tmp, "Hello", O_CREAT | O_WRONLY);
  ASSERT_EQ(-1, read(fd, &b, 1));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(0, close(fd));
}

TEST(read, example) {
  // Create pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Send string across.
  ASSERT_EQ(5, write(fds[1], "Hello", 5));
  char buf[6];
  ASSERT_EQ(5, read(fds[0], buf, sizeof(buf)));
  buf[5] = '\0';
  ASSERT_STREQ("Hello", buf);

  // Close pipe.
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

static void *sleep_then_close(void *arg) {
  // Sleep for a moment to let the read() settle.
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  EXPECT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));

  // Close the given file handle.
  EXPECT_EQ(0, close(*(int *)arg));
  return NULL;
}

TEST(read, eof) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  {
    // Create pipe.
    int fds[2];
    ASSERT_EQ(0, pipe(fds));

    // Create a thread that will close one end.
    pthread_t thread;
    ASSERT_EQ(0,
              pthread_create(&thread, NULL, sleep_then_close, (void *)&fds[0]));

    // Read from the other end. This should block until the close occurs.
    char buf[2];
    ASSERT_EQ(0, read(fds[1], buf, sizeof(buf)));

    // Clean up.
    ASSERT_EQ(0, pthread_join(thread, NULL));
    ASSERT_EQ(0, close(fds[1]));
  }

  {
    // Create file.
    int fd = openat(fd_tmp, "Hello", O_CREAT | O_RDWR);
    ASSERT_EQ(6, write(fd, "foobar", 6));

    // Read from past the end of the file. This should return EOF (0).
    ASSERT_EQ(10, lseek(fd, 10, SEEK_SET));
    char buf[2];
    ASSERT_EQ(0, read(fd, buf, sizeof(buf)));

    // Close file.
    ASSERT_EQ(0, close(fd));
  }
}
