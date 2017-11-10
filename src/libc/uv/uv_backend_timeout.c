// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <limits.h>
#include <uv.h>

int uv_backend_timeout(const uv_loop_t *loop) {
  // Poll timeout calculation as described in step 7 of libuv's design
  // overview, with the exception that the UV_RUN_NOWAIT flag is not
  // taken into account.

  // "If the loop is going to be stopped (uv_stop() was called), the
  // timeout is 0."
  if (loop->__stop)
    return 0;

  // "If there are no active handles or requests, the timeout is 0."
  if (loop->__active_ref_handles_reqs == 0)
    return 0;

  // "If there are any idle handles active, the timeout is 0."
  if (!__uv_active_idles_empty(&loop->__active_idles))
    return 0;

  // "If there are any handles pending to be closed, the timeout is 0."
  if (!__uv_closing_handles_empty(&loop->__closing_handles))
    return 0;

  // "If none of the above cases matches, the timeout of the closest
  // timer is taken, or if there are no active timers, infinity."
  const uv_timer_t *timer = __uv_active_timers_min(&loop->__active_timers);
  if (timer == NULL)
    return -1;
  if (timer->__timeout < loop->__now)
    return 0;
  uint64_t relative_timeout = timer->__timeout - loop->__now;
  if (relative_timeout > INT_MAX)
    return INT_MAX;
  return relative_timeout;
}
