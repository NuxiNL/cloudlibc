// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <stdalign.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>
#include <unistd.h>
#include <uv.h>

struct state {
  unsigned int allocs;
  unsigned int reads;
  unsigned int bytes;
  unsigned int eofs;
};

static void alloc_cb(uv_handle_t *handle, size_t suggested_size,
                     uv_buf_t *buf) {
  struct state *state = handle->data;
  ++state->allocs;

  ASSERT_EQ(65536, suggested_size);
  void *data = malloc(10);
  ASSERT_NE(NULL, data);
  *buf = uv_buf_init(data, 10);
}

static void read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  struct state *state = stream->data;
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
