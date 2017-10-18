// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>
#include <uv.h>

TEST(uv_guess_handle, bad) {
  ASSERT_EQ(UV_UNKNOWN_HANDLE, uv_guess_handle(-1));
}

TEST(uv_guess_handle, directory) {
  // libuv doesn't consider directories to be files.
  ASSERT_EQ(UV_UNKNOWN_HANDLE, uv_guess_handle(fd_tmp));
}

TEST(uv_guess_handle, regular_file) {
  int fd = openat(fd_tmp, "Hello", O_CREAT | O_WRONLY);
  ASSERT_LE(0, fd);
  ASSERT_EQ(UV_FILE, uv_guess_handle(fd));
  ASSERT_EQ(0, close(fd));
}

TEST(uv_guess_handle, socket_dgram) {
  // This implementation does not support uv_udp_t.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_DGRAM, 0, fds));
  ASSERT_EQ(UV_UNKNOWN_HANDLE, uv_guess_handle(fds[0]));
  ASSERT_EQ(UV_UNKNOWN_HANDLE, uv_guess_handle(fds[1]));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

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
