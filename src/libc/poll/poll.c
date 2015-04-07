// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <poll.h>
#include <stddef.h>

int poll(struct pollfd *fds, size_t nfds, int timeout) {
  size_t maxevents = 2 * nfds + 1;
  cloudabi_event_t events[2 * nfds + 1];
  size_t nevents = 0;

  // Construct events for poll_once().
  for (size_t i = 0; i < nfds; ++i) {
    struct pollfd *pollfd = &fds[i];
    if (pollfd->fd < 0)
      continue;
    if ((pollfd->events & POLLRDNORM) != 0) {
      cloudabi_event_t *event = &events[nevents++];
      event->userdata = pollfd;
      event->type = CLOUDABI_EVENT_TYPE_FD_READ;
      event->fd_readwrite.fd = pollfd->fd;
    }
    if ((pollfd->events & POLLWRNORM) != 0) {
      cloudabi_event_t *event = &events[nevents++];
      event->userdata = pollfd;
      event->type = CLOUDABI_EVENT_TYPE_FD_WRITE;
      event->fd_readwrite.fd = pollfd->fd;
    }
  }

  // Create extra event for the timeout.
  if (timeout >= 0) {
    cloudabi_event_t *event = &events[nevents++];
    event->type = CLOUDABI_EVENT_TYPE_CLOCK;
    event->clock.timeout = 0;
    cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, 0,
                                &event->clock.timeout);
    event->clock.timeout += (cloudabi_timestamp_t)timeout * 1000000;
    event->clock.precision = 0;
  }

  // Execute poll_once().
  cloudabi_errno_t error =
      cloudabi_sys_poll_once(events, nevents, events, maxevents, &nevents);
  if (error != 0) {
    errno = error;
    return -1;
  }

  // Clear revents fields.
  for (size_t i = 0; i < nfds; ++i) {
    struct pollfd *pollfd = &fds[i];
    pollfd->revents = 0;
  }

  // Set revents fields.
  for (size_t i = 0; i < nevents; ++i) {
    const cloudabi_event_t *event = &events[i];
    if (event->type == CLOUDABI_EVENT_TYPE_FD_READ ||
        event->type == CLOUDABI_EVENT_TYPE_FD_WRITE) {
      struct pollfd *pollfd = event->userdata;
      // TODO(edje): Add support for POLLHUP.
      if (event->error == EBADF) {
        // Invalid file descriptor.
        pollfd->revents |= POLLNVAL;
      } else if (pollfd->revents != 0) {
        // Another error occurred.
        pollfd->revents |= POLLERR;
      } else if (event->type == CLOUDABI_EVENT_TYPE_FD_READ) {
        // Data can be read.
        pollfd->revents |= POLLRDNORM;
      } else {
        // Data can be written.
        pollfd->revents |= POLLWRNORM;
      }
    }
  }

  return 0;
}
