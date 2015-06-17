// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <assert.h>
#include <errno.h>
#include <poll.h>

int poll(struct pollfd *fds, size_t nfds, int timeout) {
  // Construct events for poll().
  size_t maxevents = 2 * nfds + 1;
  cloudabi_subscription_t subscriptions[maxevents];
  size_t nevents = 0;
  for (size_t i = 0; i < nfds; ++i) {
    struct pollfd *pollfd = &fds[i];
    if (pollfd->fd < 0)
      continue;
    if ((pollfd->events & POLLRDNORM) != 0) {
      cloudabi_subscription_t *subscription = &subscriptions[nevents++];
      subscription->userdata = (uintptr_t)pollfd;
      subscription->type = CLOUDABI_EVENTTYPE_FD_READ;
      subscription->fd_readwrite.fd = pollfd->fd;
    }
    if ((pollfd->events & POLLWRNORM) != 0) {
      cloudabi_subscription_t *subscription = &subscriptions[nevents++];
      subscription->userdata = (uintptr_t)pollfd;
      subscription->type = CLOUDABI_EVENTTYPE_FD_WRITE;
      subscription->fd_readwrite.fd = pollfd->fd;
    }
  }

  // Create extra event for the timeout.
  if (timeout >= 0) {
    cloudabi_subscription_t *subscription = &subscriptions[nevents++];
    subscription->type = CLOUDABI_EVENTTYPE_CLOCK;
    subscription->clock.clock_id = CLOUDABI_CLOCK_REALTIME;
    subscription->clock.timeout = 0;
    if (timeout > 0) {
      cloudabi_sys_clock_time_get(CLOUDABI_CLOCK_REALTIME, 0,
                                  &subscription->clock.timeout);
      subscription->clock.timeout += (cloudabi_timestamp_t)timeout * 1000000;
    }
    subscription->clock.precision = 0;
  }

  // Execute poll().
  cloudabi_event_t events[nevents];
  cloudabi_errno_t error = cloudabi_sys_poll(
      CLOUDABI_POLL_ONCE, subscriptions, nevents, events, maxevents, &nevents);
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
    if (event->type == CLOUDABI_EVENTTYPE_FD_READ ||
        event->type == CLOUDABI_EVENTTYPE_FD_WRITE) {
      struct pollfd *pollfd = (struct pollfd *)(uintptr_t)event->userdata;
      assert((int)event->fd_readwrite.fd == pollfd->fd &&
             "File descriptor mismatch");
      if (event->error == CLOUDABI_EBADF) {
        // Invalid file descriptor.
        pollfd->revents = POLLNVAL;
      } else if (event->error == CLOUDABI_EPIPE) {
        // Hangup on write side of pipe.
        pollfd->revents |= POLLHUP;
      } else if (event->error != 0) {
        // Another error occurred.
        pollfd->revents = POLLERR;
      } else if (event->type == CLOUDABI_EVENTTYPE_FD_READ) {
        // Data can be read.
        pollfd->revents |= POLLRDNORM;
        if (event->fd_readwrite.flags & CLOUDABI_EVENT_FD_READWRITE_HANGUP)
          pollfd->revents |= POLLHUP;
      } else {
        // Data can be written.
        if (event->fd_readwrite.flags & CLOUDABI_EVENT_FD_READWRITE_HANGUP)
          pollfd->revents |= POLLHUP;
        else
          pollfd->revents |= POLLWRNORM;
      }
    }
  }

  // Return the number of events with a non-zero revents value.
  int retval = 0;
  for (size_t i = 0; i < nfds; ++i) {
    struct pollfd *pollfd = &fds[i];
    if (pollfd->revents != 0)
      ++retval;
  }
  return retval;
}
