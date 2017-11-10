// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <stdbool.h>
#include <uv.h>

int uv_loop_init(uv_loop_t *loop) {
  uv_update_time(loop);
  loop->__stop = false;

  __uv_handles_init(&loop->__handles);

  __uv_active_asyncs_init(&loop->__active_asyncs);
  __uv_active_checks_init(&loop->__active_checks);
  __uv_active_idles_init(&loop->__active_idles);
  __uv_active_prepares_init(&loop->__active_prepares);
  __uv_active_processes_init(&loop->__active_processes);
  __uv_active_timers_init(&loop->__active_timers);
  __uv_closing_handles_init(&loop->__closing_handles);
  __uv_reading_polls_init(&loop->__reading_polls);
  __uv_reading_streams_init(&loop->__reading_streams);
  __uv_writing_polls_init(&loop->__writing_polls);
  __uv_writing_streams_init(&loop->__writing_streams);

  loop->__active_ref_handles_reqs = 0;

  loop->__subscriptions_buffer = NULL;
  loop->__subscriptions_capacity = 0;
  loop->__events_buffer = NULL;
  loop->__events_capacity = 0;
  return 0;
}
