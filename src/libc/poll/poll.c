// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <errno.h>
#include <poll.h>
#include <stddef.h>

int poll(struct pollfd fds[], size_t nfds, int timeout) {
  size_t maxevents = 2 * nfds + 1;
  cloudabi_event_t events[2 * nfds + 1];
  size_t nevents = 0;

  // Construct events for poll_once().
  for (size_t i = 0; i < nfds; ++i) {
    struct pollfd *pollfd = &fds[i];
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
    pollfd->revents = 0;
  }

  // Create extra event for the timeout.
  if (timeout != -1) {
    cloudabi_event_t *event = &events[nevents++];
    event->type = CLOUDABI_EVENT_TYPE_CLOCK;
    // TODO(edje): Set fields.
  }

  // Execute poll_once().
  cloudabi_errno_t error =
      cloudabi_sys_poll_once(events, nevents, events, maxevents, &nevents);
  if (error != 0) {
    errno = error;
    return -1;
  }

  // Set revents fields.
  for (size_t i = 0; i < nevents; ++i) {
    // TODO(edje): Implement.
  }

  return 0;
}
