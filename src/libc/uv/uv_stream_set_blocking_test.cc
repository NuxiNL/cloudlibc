// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <fcntl.h>
#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_stream_set_blocking, example) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Cannot alter the blocking flag on unopened streams.
  uv_pipe_t pipe;
  ASSERT_EQ(0, uv_pipe_init(&loop, &pipe, 0));
  ASSERT_EQ(UV_EBADF, uv_stream_set_blocking((uv_stream_t *)&pipe, 1));

  // Socket file descriptor should be blocking by default.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(O_RDWR, fcntl(fds[0], F_GETFL));

  // Opening it as a stream should make it non-blocking.
  ASSERT_EQ(0, uv_pipe_open(&pipe, fds[0]));
  ASSERT_EQ(O_NONBLOCK | O_RDWR, fcntl(fds[0], F_GETFL));

  // The uv_stream_set_blocking() should allow us to toggle it.
  ASSERT_EQ(0, uv_stream_set_blocking((uv_stream_t *)&pipe, 1));
  ASSERT_EQ(O_RDWR, fcntl(fds[0], F_GETFL));
  ASSERT_EQ(0, uv_stream_set_blocking((uv_stream_t *)&pipe, 0));
  ASSERT_EQ(O_NONBLOCK | O_RDWR, fcntl(fds[0], F_GETFL));

  // Cannot alter the blocking flag on closed streams.
  uv_close((uv_handle_t *)&pipe, close_cb);
  ASSERT_EQ(UV_EBADF, uv_stream_set_blocking((uv_stream_t *)&pipe, 1));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}
