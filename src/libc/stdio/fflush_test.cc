// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(fflush, eagain) {
  // Create a stream to non-blocking write side of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK));
  FILE *fp = fdopen(fds[1], "w");

  // Write data into the pipe until it fails.
  flockfile(fp);
  while (putc_unlocked('a', fp) == 'a')
    ;
  funlockfile(fp);

  // Flushing should still fail with EAGAIN.
  clearerr(fp);
  errno = 0;
  ASSERT_EQ(EOF, fflush(fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EAGAIN, errno);
  ASSERT_EQ(EOF, fclose(fp));
  ASSERT_EQ(0, close(fds[0]));
}

TEST(fflush, ebadf) {
  // Create a stream to one end of a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_NE(NULL, fp);
  ASSERT_EQ('a', putc('a', fp));

  // Close underlying file descriptors.
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Flushing should fail, as underlying file descriptor is already gone.
  ASSERT_EQ(EOF, fflush(fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(EOF, fclose(fp));
}

TEST(fflush, epipe) {
  // Create a pipe of which the read side is closed.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));

  // Write data into write buffer.
  FILE *fp = fdopen(fds[1], "w");
  ASSERT_NE(NULL, fp);
  ASSERT_EQ('a', putc('a', fp));

  // We should be unable to flush.
  ASSERT_EQ(EOF, fflush(fp));
  ASSERT_TRUE(ferror(fp));
  ASSERT_EQ(EPIPE, errno);
  ASSERT_EQ(EOF, fclose(fp));
}

TEST(fflush, success) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  FILE *fp = fopenat(fd_tmp, "hello", "w+");
  ASSERT_NE(NULL, fp);

  // File descriptor position should be synced when flushing.
  ASSERT_EQ(0, fseeko(fp, 5, SEEK_SET));
  ASSERT_EQ(0, fflush(fp));
  ASSERT_EQ(5, lseek(fileno(fp), 0, SEEK_CUR));

  // Write buffer should be written when flushing.
  ASSERT_EQ(0, fputs("Hello", fp));
  ASSERT_EQ(0, fflush(fp));
  ASSERT_EQ(10, lseek(fileno(fp), 0, SEEK_CUR));
  char buf[11];
  ASSERT_EQ(10, pread(fileno(fp), buf, sizeof(buf), 0));
  buf[10] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray("\0\0\0\0\0Hello"));

  ASSERT_EQ(0, fclose(fp));
}
