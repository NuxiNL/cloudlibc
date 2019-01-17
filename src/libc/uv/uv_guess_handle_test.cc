// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(uv_guess_handle, bad) {
  ASSERT_EQ(UV_UNKNOWN_HANDLE, uv_guess_handle(-1));
}

TEST(uv_guess_handle, directory) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // libuv doesn't consider directories to be files.
  ASSERT_EQ(UV_UNKNOWN_HANDLE, uv_guess_handle(fd_tmp));
}

TEST(uv_guess_handle, regular_file) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fd = openat(fd_tmp, "Hello", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fd);
  ASSERT_EQ(UV_FILE, uv_guess_handle(fd));
  ASSERT_EQ(0, close(fd));
}

#ifdef __CloudABI__
TEST(uv_guess_handle, socket_dgram) {
  // The official implementation maps datagram sockets to
  // UV_UNKNOWN_HANDLE. In our case it makes more sense to pick UV_UDP.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_DGRAM, 0, fds));
  ASSERT_EQ(UV_UDP, uv_guess_handle(fds[0]));
  ASSERT_EQ(UV_UDP, uv_guess_handle(fds[1]));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
#endif

TEST(uv_guess_handle, socket_stream) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(UV_NAMED_PIPE, uv_guess_handle(fds[0]));
  ASSERT_EQ(UV_NAMED_PIPE, uv_guess_handle(fds[1]));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(uv_guess_handle, pipe) {
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(UV_NAMED_PIPE, uv_guess_handle(fds[0]));
  ASSERT_EQ(UV_NAMED_PIPE, uv_guess_handle(fds[1]));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
