// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

static void write_cb_success(uv_write_t *req, int status) {
  ASSERT_EQ(0, status);
  ASSERT_EQ(1, ++*(int *)req->data);
}

static void close_cb1_success(uv_handle_t *handle) {
  ASSERT_EQ(2, ++*(int *)handle->data);
}

static void close_cb2_success(uv_handle_t *handle) {
  ASSERT_EQ(3, ++*(int *)handle->data);
}

TEST(uv_write2, example) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Create a pipe that we're going to send across.
  uv_pipe_t send_handle;
  {
    int fds[2];
    ASSERT_EQ(0, pipe(fds));
    ASSERT_EQ(2, write(fds[1], "Hi", 2));
    ASSERT_EQ(0, close(fds[1]));

    ASSERT_EQ(0, uv_pipe_init(&loop, &send_handle, 0));
    ASSERT_EQ(0, uv_pipe_open(&send_handle, fds[0]));
  }

  // Create a socket pair that will send the pipe across.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  uv_pipe_t out;
  ASSERT_EQ(0, uv_pipe_init(&loop, &out, 1));
  ASSERT_EQ(0, uv_pipe_open(&out, fds[0]));

  // Schedule that the pipe is written into the socket pair.
  uv_write_t req;
  int state = 0;
  {
    uv_buf_t buf = uv_buf_init((char *)"Hello", 5);
    req.data = &state;
    ASSERT_EQ(0, uv_write2(&req, (uv_stream_t *)&out, &buf, 1,
                           (uv_stream_t *)&send_handle, write_cb_success));
  }

  // Run the loop to force writes to complete.
  // TODO(ed): The official libuv already performes writes eagerly.
  // Should we do the same?
  ASSERT_EQ(0, state);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(1, state);

  {
    // Extract data from the other end of the socket pair.
    char buf[6];
    struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
    alignas(struct cmsghdr) char control[CMSG_SPACE(sizeof(int))];
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = control,
        .msg_controllen = sizeof(control),
    };
    ASSERT_EQ(5, recvmsg(fds[1], &msg, 0));
    ASSERT_EQ(0, msg.msg_flags);
    buf[5] = '\0';
    ASSERT_STREQ("Hello", buf);
    ASSERT_EQ(0, close(fds[1]));

    // Extract the file descriptor from the control message.
    int fd;
    {
      struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
      ASSERT_NE(NULL, cmsg);
      ASSERT_EQ(CMSG_LEN(sizeof(int)), cmsg->cmsg_len);
      ASSERT_EQ(SOL_SOCKET, cmsg->cmsg_level);
      ASSERT_EQ(SCM_RIGHTS, cmsg->cmsg_type);
      ASSERT_EQ(NULL, CMSG_NXTHDR(&msg, cmsg));
      fd = *(int *)CMSG_DATA(cmsg);
    }

    // The data that was originally in the pipe should still be there.
    ASSERT_EQ(2, read(fd, buf, sizeof(buf)));
    buf[2] = '\0';
    ASSERT_STREQ("Hi", buf);
    ASSERT_EQ(0, close(fd));
  }

  // Schedule that the pipe and socket pair are closed.
  out.data = &state;
  uv_close((uv_handle_t *)&out, close_cb2_success);
  send_handle.data = &state;
  uv_close((uv_handle_t *)&send_handle, close_cb1_success);

  ASSERT_EQ(1, state);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(3, state);
  ASSERT_EQ(0, uv_loop_close(&loop));
}
