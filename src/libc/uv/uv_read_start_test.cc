// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>

#include "gtest/gtest.h"

struct state {
  unsigned int allocs;
  unsigned int reads;
  unsigned int bytes;
  unsigned int eofs;
};

static void alloc_cb(uv_handle_t *handle, size_t suggested_size,
                     uv_buf_t *buf) {
  auto state = static_cast<struct state *>(handle->data);
  ++state->allocs;

  ASSERT_EQ(65536, suggested_size);
  auto data = static_cast<char *>(malloc(10));
  ASSERT_NE(NULL, data);
  *buf = uv_buf_init(data, 10);
}

static void read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  auto state = static_cast<struct state *>(stream->data);
  ++state->reads;
  if (nread > 0) {
    for (ssize_t i = 0; i < nread; ++i)
      ASSERT_EQ('A' + state->bytes++ % 26, buf->base[i]);
  } else {
    ++state->eofs;
    ASSERT_EQ(UV_EOF, nread);
  }
  free(buf->base);
}

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_read_start, ipc_disabled) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  uv_pipe_t pipe;
  ASSERT_EQ(0, uv_pipe_init(&loop, &pipe, 0));
  ASSERT_EQ(0, uv_pipe_open(&pipe, fds[0]));

  // There should be no data available for reading initially.
  struct state state = {};
  pipe.data = &state;
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&pipe, alloc_cb, read_cb));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_NOWAIT));
  ASSERT_EQ(0, state.allocs);
  ASSERT_EQ(0, state.reads);
  ASSERT_EQ(0, state.bytes);
  ASSERT_EQ(0, state.eofs);

  // Perform multiple writes that in total are smaller than the read
  // buffer size. Any file descriptors attached to the payload should be
  // discarded, as the stream has IPC disabled. Still, there should be
  // artificial read boundaries introduced by where the file descriptors
  // were.
  ASSERT_EQ(1, write(fds[1], "A", 1));
  {
    struct iovec iov = {.iov_base = (char *)"BC", .iov_len = 2};
    alignas(struct cmsghdr) char control[CMSG_SPACE(sizeof(fds))];
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = control,
        .msg_controllen = sizeof(control),
    };
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(fds));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    memcpy(CMSG_DATA(cmsg), fds, sizeof(fds));
    ASSERT_EQ(2, sendmsg(fds[1], &msg, 0));
  }
  ASSERT_EQ(2, write(fds[1], "DE", 2));
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&pipe, alloc_cb, read_cb));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_ONCE));
  ASSERT_EQ(1, state.allocs);
  ASSERT_EQ(1, state.reads);
  ASSERT_EQ(1, state.bytes);
  ASSERT_EQ(0, state.eofs);
  ASSERT_EQ(0, uv_pipe_pending_count(&pipe));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_ONCE));
  ASSERT_EQ(2, state.allocs);
  ASSERT_EQ(2, state.reads);
  ASSERT_EQ(5, state.bytes);
  ASSERT_EQ(0, state.eofs);
  ASSERT_EQ(0, uv_pipe_pending_count(&pipe));

  // Write more than a single buffer of data. This should be split up in
  // multiple reads.
  ASSERT_EQ(16, write(fds[1], "FGHIJKLMNOPQRSTU", 16));
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&pipe, alloc_cb, read_cb));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_ONCE));
  ASSERT_EQ(4, state.allocs);
  ASSERT_EQ(4, state.reads);
  ASSERT_EQ(21, state.bytes);
  ASSERT_EQ(0, state.eofs);
  ASSERT_EQ(0, uv_pipe_pending_count(&pipe));

  // Terminate the input. The loop should now stop, as there is nothing
  // left to do.
  ASSERT_EQ(5, write(fds[1], "VWXYZ", 5));
  ASSERT_EQ(0, close(fds[1]));
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(6, state.allocs);
  ASSERT_EQ(6, state.reads);
  ASSERT_EQ(26, state.bytes);
  ASSERT_EQ(1, state.eofs);
  ASSERT_EQ(0, uv_pipe_pending_count(&pipe));

  uv_close((uv_handle_t *)&pipe, close_cb);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}

TEST(uv_read_start, ipc_enabled) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  uv_pipe_t stream;
  ASSERT_EQ(0, uv_pipe_init(&loop, &stream, 1));
  ASSERT_EQ(0, uv_pipe_open(&stream, fds[0]));

  // Write two groups of two file descriptors into the socket. Each of
  // the file descriptors is a pipe containing a short message.
  {
    struct iovec iov = {.iov_base = (char *)"ABCDE", .iov_len = 5};
    alignas(struct cmsghdr) char control[CMSG_SPACE(sizeof(fds))];
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = control,
        .msg_controllen = sizeof(control),
    };
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(fds));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    int *data = (int *)CMSG_DATA(cmsg);
    {
      int mfds[2];
      ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, mfds));
      ASSERT_EQ(3, write(mfds[1], "Foo", 3));
      ASSERT_EQ(0, close(mfds[1]));
      data[0] = mfds[0];
    }
    {
      int mfds[2];
      ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, mfds));
      ASSERT_EQ(3, write(mfds[1], "Bar", 3));
      ASSERT_EQ(0, close(mfds[1]));
      data[1] = mfds[0];
    }
    ASSERT_EQ(5, sendmsg(fds[1], &msg, 0));
    ASSERT_EQ(0, close(data[0]));
    ASSERT_EQ(0, close(data[1]));
  }
  {
    struct iovec iov = {.iov_base = (char *)"FGHIJ", .iov_len = 5};
    alignas(struct cmsghdr) char control[CMSG_SPACE(sizeof(fds))];
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = control,
        .msg_controllen = sizeof(control),
    };
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_len = CMSG_LEN(sizeof(fds));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    int *data = (int *)CMSG_DATA(cmsg);
    {
      int mfds[2];
      ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, mfds));
      ASSERT_EQ(3, write(mfds[1], "Baz", 3));
      ASSERT_EQ(0, close(mfds[1]));
      data[0] = mfds[0];
    }
    {
      int mfds[2];
      ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, mfds));
      ASSERT_EQ(3, write(mfds[1], "Qux", 3));
      ASSERT_EQ(0, close(mfds[1]));
      data[1] = mfds[0];
    }
    ASSERT_EQ(5, sendmsg(fds[1], &msg, 0));
    ASSERT_EQ(0, close(data[0]));
    ASSERT_EQ(0, close(data[1]));
  }
  ASSERT_EQ(0, close(fds[1]));

  // We should be able to read them back in two runs.
  struct state state = {};
  stream.data = &state;
  ASSERT_EQ(0, uv_read_start((uv_stream_t *)&stream, alloc_cb, read_cb));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_ONCE));
  ASSERT_EQ(1, state.allocs);
  ASSERT_EQ(1, state.reads);
  ASSERT_EQ(5, state.bytes);
  ASSERT_EQ(0, state.eofs);
  ASSERT_EQ(2, uv_pipe_pending_count(&stream));
  ASSERT_EQ(1, uv_run(&loop, UV_RUN_ONCE));
  ASSERT_EQ(2, state.allocs);
  ASSERT_EQ(2, state.reads);
  ASSERT_EQ(10, state.bytes);
  ASSERT_EQ(0, state.eofs);
  ASSERT_EQ(4, uv_pipe_pending_count(&stream));

  // Extract three of the four file descriptors.
  ASSERT_EQ(UV_NAMED_PIPE, uv_pipe_pending_type(&stream));
  uv_pipe_t received1;
  ASSERT_EQ(0, uv_pipe_init(&loop, &received1, 0));
  ASSERT_EQ(0, uv_accept((uv_stream_t *)&stream, (uv_stream_t *)&received1));
  {
    int fd;
    ASSERT_EQ(0, uv_fileno((uv_handle_t *)&received1, &fd));
    char buf[4];
    ASSERT_EQ(3, read(fd, buf, sizeof(buf)));
    buf[3] = '\0';
    ASSERT_STREQ("Foo", buf);
  }
  uv_close((uv_handle_t *)&received1, close_cb);
  ASSERT_EQ(3, uv_pipe_pending_count(&stream));

  ASSERT_EQ(UV_NAMED_PIPE, uv_pipe_pending_type(&stream));
  uv_pipe_t received2;
  ASSERT_EQ(0, uv_pipe_init(&loop, &received2, 0));
  ASSERT_EQ(0, uv_accept((uv_stream_t *)&stream, (uv_stream_t *)&received2));
  {
    int fd;
    ASSERT_EQ(0, uv_fileno((uv_handle_t *)&received2, &fd));
    char buf[4];
    ASSERT_EQ(3, read(fd, buf, sizeof(buf)));
    buf[3] = '\0';
    ASSERT_STREQ("Bar", buf);
  }
  uv_close((uv_handle_t *)&received2, close_cb);
  ASSERT_EQ(2, uv_pipe_pending_count(&stream));

  ASSERT_EQ(UV_NAMED_PIPE, uv_pipe_pending_type(&stream));
  uv_pipe_t received3;
  ASSERT_EQ(0, uv_pipe_init(&loop, &received3, 0));
  ASSERT_EQ(0, uv_accept((uv_stream_t *)&stream, (uv_stream_t *)&received3));
  {
    int fd;
    ASSERT_EQ(0, uv_fileno((uv_handle_t *)&received3, &fd));
    char buf[4];
    ASSERT_EQ(3, read(fd, buf, sizeof(buf)));
    buf[3] = '\0';
    ASSERT_STREQ("Baz", buf);
  }
  uv_close((uv_handle_t *)&received3, close_cb);
  ASSERT_EQ(1, uv_pipe_pending_count(&stream));

  // Clean up.
  uv_close((uv_handle_t *)&stream, close_cb);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));
}
