// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_fileno, einval) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  uv_idle_t handle;
  ASSERT_EQ(0, uv_idle_init(&loop, &handle));
  uv_os_fd_t fd;
  ASSERT_EQ(UV_EINVAL, uv_fileno((uv_handle_t *)&handle, &fd));

  uv_close((uv_handle_t *)&handle, close_cb);
  ASSERT_EQ(UV_EINVAL, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(UV_EINVAL, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_loop_close(&loop));
}

TEST(uv_fileno, pipe) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  uv_pipe_t handle;
  ASSERT_EQ(0, uv_pipe_init(&loop, &handle, 0));
  uv_os_fd_t fd;
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(0, uv_pipe_open(&handle, fds[0]));
  ASSERT_EQ(0, uv_fileno((uv_handle_t *)&handle, &fd));
  ASSERT_EQ(fds[0], fd);

  uv_close((uv_handle_t *)&handle, close_cb);
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_loop_close(&loop));
}

TEST(uv_fileno, poll) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(0, close(fds[1]));
  uv_poll_t handle;
  ASSERT_EQ(0, uv_poll_init(&loop, &handle, fds[0]));
  uv_os_fd_t fd;
  ASSERT_EQ(0, uv_fileno((uv_handle_t *)&handle, &fd));
  ASSERT_EQ(fds[0], fd);

  uv_close((uv_handle_t *)&handle, close_cb);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_loop_close(&loop));
}

TEST(uv_fileno, tcp) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  uv_tcp_t handle;
  ASSERT_EQ(0, uv_tcp_init(&loop, &handle));
  uv_os_fd_t fd;
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(0, uv_tcp_open(&handle, fds[0]));
  ASSERT_EQ(0, uv_fileno((uv_handle_t *)&handle, &fd));
  ASSERT_EQ(fds[0], fd);

  uv_close((uv_handle_t *)&handle, close_cb);
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(UV_EBADF, uv_fileno((uv_handle_t *)&handle, &fd));

  ASSERT_EQ(0, uv_loop_close(&loop));
}
