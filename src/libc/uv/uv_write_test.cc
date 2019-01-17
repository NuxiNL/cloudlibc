// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <unistd.h>
#include <uv.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

static void write_cb_epipe(uv_write_t *req, int status) {
  ASSERT_EQ(UV_EPIPE, status);
  ASSERT_EQ(1, ++*(int *)req->data);
}

static void close_cb_epipe(uv_handle_t *handle) {
  ASSERT_EQ(2, ++*(int *)handle->data);
}

TEST(uv_write, epipe) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[0]));
  uv_pipe_t out;
  ASSERT_EQ(0, uv_pipe_init(&loop, &out, 0));
  ASSERT_EQ(0, uv_pipe_open(&out, fds[1]));

  // Writing to the close pipe should generate EPIPE.
  uv_buf_t buf = uv_buf_init((char *)"Hello", 5);
  uv_write_t req;
  int state = 0;
  req.data = &state;
  ASSERT_EQ(0, uv_write(&req, (uv_stream_t *)&out, &buf, 1, write_cb_epipe));

  // Run the loop to force writes to complete.
  ASSERT_EQ(0, state);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(1, state);

  // Schedule that the pipe is closed.
  out.data = &state;
  uv_close((uv_handle_t *)&out, close_cb_epipe);

  ASSERT_EQ(1, state);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(2, state);
  ASSERT_EQ(0, uv_loop_close(&loop));
}

// Use a state number to validate that callbacks are actually run.
static void write_cb1_success(uv_write_t *req, int status) {
  ASSERT_EQ(0, status);
  ASSERT_EQ(1, ++*(int *)req->data);
}

static void write_cb2_success(uv_write_t *req, int status) {
  ASSERT_EQ(0, status);
  ASSERT_EQ(2, ++*(int *)req->data);
}

static void close_cb_success(uv_handle_t *handle) {
  ASSERT_EQ(3, ++*(int *)handle->data);
}

TEST(uv_write, example) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Attach to a pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  uv_pipe_t out;
  ASSERT_EQ(0, uv_pipe_init(&loop, &out, 0));
  ASSERT_EQ(0, uv_pipe_open(&out, fds[1]));

  // Schedule that data is written into the pipe.
  uv_buf_t bufs1[2] = {
      uv_buf_init((char *)"Hello ", 6),
      uv_buf_init((char *)"World!\n", 7),
  };
  uv_write_t req1;
  int state = 0;
  req1.data = &state;
  ASSERT_EQ(0,
            uv_write(&req1, (uv_stream_t *)&out, bufs1, 2, write_cb1_success));

  uv_buf_t bufs2[2] = {
      uv_buf_init((char *)"Goodbye ", 8),
      uv_buf_init((char *)"World!\n", 7),
  };
  uv_write_t req2;
  req2.data = &state;
  ASSERT_EQ(0,
            uv_write(&req2, (uv_stream_t *)&out, bufs2, 2, write_cb2_success));

  // Run the loop to force writes to complete.
  // TODO(ed): The official libuv already performes writes eagerly.
  // Should we do the same?
  ASSERT_EQ(0, state);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(2, state);

  // Extract data from the other end of the pipe.
  char buf[29];
  ASSERT_EQ(28, read(fds[0], buf, sizeof(buf)));
  buf[28] = '\0';
  ASSERT_THAT(buf, testing::ElementsAreArray("Hello World!\nGoodbye World!\n"));
  ASSERT_EQ(0, close(fds[0]));

  // Schedule that the pipe is closed.
  out.data = &state;
  uv_close((uv_handle_t *)&out, close_cb_success);

  ASSERT_EQ(2, state);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(3, state);
  ASSERT_EQ(0, uv_loop_close(&loop));
}
