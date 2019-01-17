// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_is_readable, example) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Unopened stream should not be readable.
  uv_pipe_t stream;
  ASSERT_EQ(0, uv_pipe_init(&loop, &stream, 0));
  ASSERT_FALSE(uv_is_readable((uv_stream_t *)&stream));

  // Opening it with a pipe file descriptor will make it readable.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(0, uv_pipe_open(&stream, fds[0]));
  ASSERT_TRUE(uv_is_readable((uv_stream_t *)&stream));

  // Closing it should still leave it readable.
  uv_close((uv_handle_t *)&stream, close_cb);
  ASSERT_TRUE(uv_is_readable((uv_stream_t *)&stream));

  // Even when closed, it should be considered readable.
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_TRUE(uv_is_readable((uv_stream_t *)&stream));

  ASSERT_EQ(0, uv_loop_close(&loop));
}
