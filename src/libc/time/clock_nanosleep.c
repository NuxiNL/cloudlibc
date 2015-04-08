// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <errno.h>
#include <time.h>

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp,
                    ...) {
  // Prepare polling event.
  cloudabi_event_t ev = {
      .type = CLOUDABI_EVENT_TYPE_CLOCK, .clock.clock_id = clock_id,
  };
  if (!timespec_to_timestamp_clamp(rqtp, &ev.clock.timeout))
    return EINVAL;

  switch (flags) {
    case 0: {
      // Transform to absolute timeout.
      cloudabi_timestamp_t now;
      cloudabi_errno_t error = cloudabi_sys_clock_time_get(clock_id, 1, &now);
      if (error != 0)
        return ENOTSUP;

      // Ensure that addition does not cause an overflow.
      if (now > NUMERIC_MAX(cloudabi_timestamp_t) - ev.clock.timeout)
        return EINVAL;
      ev.clock.timeout += now;
      break;
    }
    case TIMER_ABSTIME:
      // Timestamp is already absolute.
      break;
    default:
      return EINVAL;
  }

  // Block until polling event is triggered.
  size_t nevents;
  cloudabi_errno_t error =
      cloudabi_sys_poll(CLOUDABI_POLL_ONCE, &ev, 1, &ev, 1, &nevents);
  return error == 0 && ev.error == 0 ? 0 : ENOTSUP;
}
