// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <testing.h>
#include <uv.h>

TEST(uv_req_size, example) {
  ASSERT_EQ(-1, uv_req_size(UV_UNKNOWN_REQ));
  ASSERT_EQ(sizeof(uv_req_t), uv_req_size(UV_REQ));
  ASSERT_EQ(sizeof(uv_write_t), uv_req_size(UV_WRITE));
  ASSERT_EQ(sizeof(uv_shutdown_t), uv_req_size(UV_SHUTDOWN));
  ASSERT_EQ(sizeof(uv_fs_t), uv_req_size(UV_FS));
  ASSERT_EQ(sizeof(uv_work_t), uv_req_size(UV_WORK));
  ASSERT_EQ(sizeof(uv_getaddrinfo_t), uv_req_size(UV_GETADDRINFO));
  ASSERT_EQ(sizeof(uv_getnameinfo_t), uv_req_size(UV_GETNAMEINFO));
  ASSERT_EQ(-1, uv_req_size(UV_REQ_TYPE_MAX));
}
