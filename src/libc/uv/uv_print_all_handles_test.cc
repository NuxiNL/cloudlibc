// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

static void async_cb(uv_async_t *async) {
}

static void close_cb(uv_handle_t *handle) {
}

TEST(uv_print_all_handles, example) {
  uv_loop_t loop;
  ASSERT_EQ(0, uv_loop_init(&loop));

  // Insert a couple of handles into the loop.
  uv_idle_t idle1;
  ASSERT_EQ(0, uv_idle_init(&loop, &idle1));
  uv_prepare_t prepare1;
  ASSERT_EQ(0, uv_prepare_init(&loop, &prepare1));
  uv_async_t async1;
  ASSERT_EQ(0, uv_async_init(&loop, &async1, async_cb));
  uv_idle_t idle2;
  ASSERT_EQ(0, uv_idle_init(&loop, &idle2));
  uv_prepare_t prepare2;
  ASSERT_EQ(0, uv_prepare_init(&loop, &prepare2));
  uv_unref((uv_handle_t *)&prepare2);

  // Print all of the handles into a memory buffer.
  char *buf;
  size_t buflen;
  FILE *fp = open_memstream(&buf, &buflen);
  ASSERT_NE(NULL, fp);
  uv_print_all_handles(&loop, fp);
  ASSERT_EQ(0, fclose(fp));

  // Clean up the loop.
  uv_close((uv_handle_t *)&idle1, close_cb);
  uv_close((uv_handle_t *)&prepare1, close_cb);
  uv_close((uv_handle_t *)&async1, close_cb);
  uv_close((uv_handle_t *)&idle2, close_cb);
  uv_close((uv_handle_t *)&prepare2, close_cb);
  ASSERT_EQ(0, uv_run(&loop, UV_RUN_DEFAULT));
  ASSERT_EQ(0, uv_loop_close(&loop));

  // The loop may print some internal handles at first, but at the end
  // of the output, we should get a list of all of the handles we
  // registered ourselves.
  char *expected;
  asprintf(&expected,
           "[R--] idle     %p\n"
           "[R--] prepare  %p\n"
           "[RA-] async    %p\n"
           "[R--] idle     %p\n"
           "[---] prepare  %p\n",
           &idle1, &prepare1, &async1, &idle2, &prepare2);
  ASSERT_THAT(buf, testing::EndsWith(expected));

  free(buf);
  free(expected);
}
