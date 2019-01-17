// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

#include "gtest/gtest.h"

TEST(uv_handle_size, example) {
  ASSERT_EQ(-1, uv_handle_size(UV_UNKNOWN_HANDLE));
  ASSERT_EQ(sizeof(uv_async_t), uv_handle_size(UV_ASYNC));
  ASSERT_EQ(sizeof(uv_check_t), uv_handle_size(UV_CHECK));
  ASSERT_EQ(sizeof(uv_handle_t), uv_handle_size(UV_HANDLE));
  ASSERT_EQ(sizeof(uv_idle_t), uv_handle_size(UV_IDLE));
  ASSERT_EQ(sizeof(uv_pipe_t), uv_handle_size(UV_NAMED_PIPE));
  ASSERT_EQ(sizeof(uv_poll_t), uv_handle_size(UV_POLL));
  ASSERT_EQ(sizeof(uv_prepare_t), uv_handle_size(UV_PREPARE));
  ASSERT_EQ(sizeof(uv_process_t), uv_handle_size(UV_PROCESS));
  ASSERT_EQ(sizeof(uv_stream_t), uv_handle_size(UV_STREAM));
  ASSERT_EQ(sizeof(uv_tcp_t), uv_handle_size(UV_TCP));
  ASSERT_EQ(sizeof(uv_tty_t), uv_handle_size(UV_TTY));
  ASSERT_EQ(sizeof(uv_timer_t), uv_handle_size(UV_TIMER));
  ASSERT_EQ(-1, uv_handle_size(UV_FILE));
  ASSERT_EQ(-1, uv_handle_size(UV_HANDLE_TYPE_MAX));
}
