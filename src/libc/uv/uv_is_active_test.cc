// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <stdbool.h>
#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void close_noop(uv_handle_t *handle) {
}

static void async_never_called(uv_async_t *handle) {
  ASSERT_TRUE(false);
}

TEST(uv_is_active, async) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Async object should be active right after creation.
  uv_async_t async;
  ASSERT_EQ(0, uv_async_init(&loop, &async, async_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&async));

  // Closing the async object should deactivate it.
  uv_close((uv_handle_t *)&async, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&async));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

static void check_never_called(uv_check_t *handle) {
  ASSERT_TRUE(false);
}

TEST(uv_is_active, check) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Check objects should not be active upon creation.
  uv_check_t check;
  ASSERT_EQ(0, uv_check_init(&loop, &check));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&check));

  // Starting the check will make it active.
  ASSERT_EQ(0, uv_check_start(&check, check_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&check));

  // Stopping the check will make it inactive again.
  ASSERT_EQ(0, uv_check_stop(&check));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&check));

  // Closing the check, even when active, should deactivate it.
  ASSERT_EQ(0, uv_check_start(&check, check_never_called));
  uv_close((uv_handle_t *)&check, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&check));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

static void idle_never_called(uv_idle_t *handle) {
  ASSERT_TRUE(false);
}

TEST(uv_is_active, idle) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Idle objects should not be active upon creation.
  uv_idle_t idle;
  ASSERT_EQ(0, uv_idle_init(&loop, &idle));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&idle));

  // Starting the idle will make it active.
  ASSERT_EQ(0, uv_idle_start(&idle, idle_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&idle));

  // Stopping the idle will make it inactive again.
  ASSERT_EQ(0, uv_idle_stop(&idle));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&idle));

  // Closing the idle, even when active, should deactivate it.
  ASSERT_EQ(0, uv_idle_start(&idle, idle_never_called));
  uv_close((uv_handle_t *)&idle, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&idle));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

static void stream_alloc_never_called(uv_handle_t *handle,
                                      size_t suggested_size, uv_buf_t *buf) {
  ASSERT_TRUE(false);
}

static void stream_read_never_called(uv_stream_t *stream, ssize_t nread,
                                     const uv_buf_t *buf) {
  ASSERT_TRUE(false);
}

TEST(uv_is_active, pipe) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Create a file descriptor for the pipe object.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[1]));

  // Streams should not be active upon creation.
  uv_pipe_t pipe;
  ASSERT_EQ(0, uv_pipe_init(&loop, &pipe, 0));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&pipe));
  ASSERT_EQ(0, uv_pipe_open(&pipe, fds[0]));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&pipe));

  // TODO(ed): Add testing coverage for uv_shutdown_t and uv_write_t.
  // These may be executed immediately, leaving the stream inactive.

  // Reading from the stream will make it active.
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&pipe, stream_alloc_never_called,
                             stream_read_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&pipe));

  // Stop reading the stream will make it inactive again.
  ASSERT_EQ(0, uv_read_stop((uv_stream_t *)&pipe));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&pipe));

  // Closing the stream, even when reading, should deactivate it.
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&pipe, stream_alloc_never_called,
                             stream_read_never_called));
  uv_close((uv_handle_t *)&pipe, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&pipe));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

static void poll_never_called(uv_poll_t *handle, int status, int events) {
  ASSERT_TRUE(false);
}

TEST(uv_is_active, poll) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Create a file descriptor for polling.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[1]));

  // Poll objects should not be active upon creation.
  uv_poll_t poll;
  ASSERT_EQ(0, uv_poll_init(&loop, &poll, fds[0]));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&poll));

  // Starting the poll will make it active.
  ASSERT_EQ(0, uv_poll_start(&poll, UV_READABLE, poll_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&poll));

  // Stopping the poll will make it inactive again.
  ASSERT_EQ(0, uv_poll_stop(&poll));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&poll));

  // Setting events to zero should not cause it to be started.
  ASSERT_EQ(0, uv_poll_start(&poll, 0, poll_never_called));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&poll));

  // Closing the poll, even when active, should deactivate it.
  ASSERT_EQ(0, uv_poll_start(&poll, UV_READABLE, poll_never_called));
  uv_close((uv_handle_t *)&poll, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&poll));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));

  ASSERT_EQ(0, close(fds[0]));
}

static void prepare_never_called(uv_prepare_t *handle) {
  ASSERT_TRUE(false);
}

TEST(uv_is_active, prepare) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Prepare objects should not be active upon creation.
  uv_prepare_t prepare;
  ASSERT_EQ(0, uv_prepare_init(&loop, &prepare));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&prepare));

  // Starting the prepare will make it active.
  ASSERT_EQ(0, uv_prepare_start(&prepare, prepare_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&prepare));

  // Stopping the prepare will make it inactive again.
  ASSERT_EQ(0, uv_prepare_stop(&prepare));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&prepare));

  // Closing the prepare, even when active, should deactivate it.
  ASSERT_EQ(0, uv_prepare_start(&prepare, prepare_never_called));
  uv_close((uv_handle_t *)&prepare, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&prepare));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

// TODO(ed): Add tests for uv_process_t!

TEST(uv_is_active, tcp) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Create a file descriptor for the tcp object.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, close(fds[1]));

  // Streams should not be active upon creation.
  uv_tcp_t tcp;
  ASSERT_EQ(0, uv_tcp_init(&loop, &tcp));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&tcp));
  ASSERT_EQ(0, uv_tcp_open(&tcp, fds[0]));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&tcp));

  // TODO(ed): Add testing coverage for uv_shutdown_t and uv_write_t.
  // These may be executed immediately, leaving the stream inactive.

  // Reading from the stream will make it active.
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&tcp, stream_alloc_never_called,
                             stream_read_never_called));
  ASSERT_TRUE(uv_is_active((uv_handle_t *)&tcp));

  // Stop reading the stream will make it inactive again.
  ASSERT_EQ(0, uv_read_stop((uv_stream_t *)&tcp));
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&tcp));

  // Closing the stream, even when reading, should deactivate it.
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&tcp, stream_alloc_never_called,
                             stream_read_never_called));
  uv_close((uv_handle_t *)&tcp, close_noop);
  ASSERT_FALSE(uv_is_active((uv_handle_t *)&tcp));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

// TODO(ed): Add tests for uv_timer_t!
