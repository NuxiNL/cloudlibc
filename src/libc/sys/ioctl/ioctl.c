// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/ioctl.h>

#include <errno.h>
#include <stdarg.h>

int ioctl(int fildes, unsigned long request, ...) {
  switch (request) {
    case FIONREAD: {
      // Poll the file descriptor to determine how many bytes can be read.
      cloudabi_subscription_t subscriptions[2] = {
          {
              .type = CLOUDABI_EVENTTYPE_FD_READ, .fd_readwrite.fd = fildes,
          },
          {
              .type = CLOUDABI_EVENTTYPE_CLOCK,
              .clock.clock_id = CLOUDABI_CLOCK_MONOTONIC,
          },
      };
      cloudabi_event_t events[__arraycount(subscriptions)];
      size_t nevents;
      cloudabi_errno_t error = cloudabi_sys_poll(
          subscriptions, events, __arraycount(subscriptions), &nevents);
      if (error != 0) {
        errno = error;
        return -1;
      }

      // Location where result should be written.
      va_list ap;
      va_start(ap, request);
      int *result = va_arg(ap, int *);
      va_end(ap);

      // Extract number of bytes for reading from poll results.
      for (size_t i = 0; i < nevents; ++i) {
        cloudabi_event_t *event = &events[i];
        if (event->error != 0) {
          errno = event->error;
          return -1;
        }
        if (event->type == CLOUDABI_EVENTTYPE_FD_READ) {
          *result = event->fd_readwrite.nbytes;
          return 0;
        }
      }

      // No data available for reading.
      *result = 0;
      return 0;
    }
    default:
      // Invalid request.
      errno = EINVAL;
      return -1;
  }
}
