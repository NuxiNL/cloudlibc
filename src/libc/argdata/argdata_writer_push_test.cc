// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <argdata.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

// TODO(ed): Add more tests!

TEST(argdata_writer_push, ebadf) {
  // Invalid file descriptor.
  argdata_writer_t *aw = argdata_writer_create();
  ASSERT_NE(NULL, aw);
  argdata_writer_set(aw, &argdata_null);
  ASSERT_EQ(EBADF, argdata_writer_push(aw, -123));
  argdata_writer_free(aw);
}

TEST(argdata_writer_push, enotsock) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fd = openat(fd_tmp, "reg1", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fd);

  // Attempting to push a file descriptor through a regular file.
  argdata_writer_t *aw = argdata_writer_create();
  ASSERT_NE(NULL, aw);
  argdata_t *ad = argdata_create_fd(fd_tmp);
  ASSERT_NE(NULL, ad);
  argdata_writer_set(aw, ad);
  ASSERT_EQ(ENOTSOCK, argdata_writer_push(aw, fd));
  argdata_writer_free(aw);
  argdata_free(ad);

  ASSERT_EQ(0, close(fd));
}

TEST(argdata_writer_push, epipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));

  // Writing to a closed pipe.
  argdata_writer_t *aw = argdata_writer_create();
  ASSERT_NE(NULL, aw);
  argdata_writer_set(aw, &argdata_null);
  ASSERT_EQ(EPIPE, argdata_writer_push(aw, fds[1]));
  argdata_writer_free(aw);

  ASSERT_EQ(0, close(fds[1]));
}

TEST(argdata_writer_push, idle) {
  // Pushing without any message enqueued should be a no-op.
  argdata_writer_t *aw = argdata_writer_create();
  ASSERT_NE(NULL, aw);
  ASSERT_EQ(0, argdata_writer_push(aw, -123));
  ASSERT_EQ(0, argdata_writer_push(aw, -123));
  ASSERT_EQ(0, argdata_writer_push(aw, -123));
  argdata_writer_free(aw);
}

TEST(argdata_writer_push, pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Write a small number of messages into a pipe.
  argdata_writer_t *aw = argdata_writer_create();
  ASSERT_NE(NULL, aw);
  argdata_writer_set(aw, &argdata_null);
  ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
  argdata_writer_set(aw, &argdata_true);
  ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
  argdata_writer_set(aw, &argdata_false);
  ASSERT_EQ(0, argdata_writer_push(aw, fds[1]));
  argdata_writer_free(aw);
  ASSERT_EQ(0, close(fds[1]));

  // Extract messages in serialized form.
  char buf[28];
  ASSERT_EQ(27, read(fds[0], buf, sizeof(buf)));
  buf[27] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray(
                       // Null.
                       "\x00\x00\x00\x00\x00\x00\x00\x00"
                       // True.
                       "\x00\x00\x00\x00\x00\x00\x00\x02\x02\x01"
                       // False.
                       "\x00\x00\x00\x00\x00\x00\x00\x01\x02"));
  ASSERT_EQ(0, close(fds[0]));
}
