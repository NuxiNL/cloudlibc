// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_is_writable, example) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Unopened stream should not be writable.
  uv_pipe_t stream;
  ASSERT_EQ(0, uv_pipe_init(&loop, &stream, 0));
  ASSERT_FALSE(uv_is_writable((uv_stream_t *)&stream));

  // Opening it with a pipe file descriptor will make it writable.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, uv_pipe_open(&stream, fds[1]));
  ASSERT_TRUE(uv_is_writable((uv_stream_t *)&stream));

  // Closing it should still leave it writable.
  uv_close((uv_handle_t *)&stream, close_cb);
  ASSERT_TRUE(uv_is_writable((uv_stream_t *)&stream));

  // Even when closed, it should be considered writable.
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_TRUE(uv_is_writable((uv_stream_t *)&stream));

  ASSERT_EQ(0, uv_loop_close(&loop));
}
