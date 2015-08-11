// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>
#include <common/time.h>

#include <sys/select.h>

#include <errno.h>

int pselect(int nfds, fd_set *restrict readfds, fd_set *restrict writefds,
            fd_set *restrict errorfds, const struct timespec *restrict timeout,
            ...) {
  // Negative file descriptor upperbound.
  if (nfds < 0) {
    errno = EINVAL;
    return -1;
  }

  // Determine the maximum number of events.
  size_t maxevents = 1;
  if (readfds != NULL)
    maxevents += readfds->__nfds;
  if (writefds != NULL)
    maxevents += writefds->__nfds;
  cloudabi_subscription_t subscriptions[maxevents];

  // Convert the readfds set.
  size_t nevents = 0;
  if (readfds != NULL) {
    for (size_t i = 0; i < readfds->__nfds; ++i) {
      int fd = readfds->__fds[i];
      if (fd < nfds) {
        cloudabi_subscription_t *subscription = &subscriptions[nevents++];
        *subscription = (cloudabi_subscription_t){
            .type = CLOUDABI_EVENTTYPE_FD_READ,
            .fd_readwrite.fd = fd,
            .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
        };
      }
    }
  }

  // Convert the writefds set.
  if (writefds != NULL) {
    for (size_t i = 0; i < writefds->__nfds; ++i) {
      int fd = writefds->__fds[i];
      if (fd < nfds) {
        cloudabi_subscription_t *subscription = &subscriptions[nevents++];
        *subscription = (cloudabi_subscription_t){
            .type = CLOUDABI_EVENTTYPE_FD_WRITE,
            .fd_readwrite.fd = fd,
            .fd_readwrite.flags = CLOUDABI_SUBSCRIPTION_FD_READWRITE_POLL,
        };
      }
    }
  }

  // Create extra event for the timeout.
  if (timeout != NULL) {
    cloudabi_subscription_t *subscription = &subscriptions[nevents++];
    *subscription = (cloudabi_subscription_t){
        .type = CLOUDABI_EVENTTYPE_CLOCK,
        .clock.clock_id = CLOUDABI_CLOCK_REALTIME,
    };
    if (!timespec_to_timestamp_clamp(timeout, &subscription->clock.timeout)) {
      errno = EINVAL;
      return -1;
    }
  }

  // Execute poll().
  cloudabi_event_t events[nevents];
  cloudabi_errno_t error =
      cloudabi_sys_poll(subscriptions, events, nevents, &nevents);
  if (error != 0) {
    errno = error;
    return -1;
  }

  // Test for EBADF.
  for (size_t i = 0; i < nevents; ++i) {
    const cloudabi_event_t *event = &events[i];
    if ((event->type == CLOUDABI_EVENTTYPE_FD_READ ||
         event->type == CLOUDABI_EVENTTYPE_FD_WRITE) &&
        event->error == CLOUDABI_EBADF) {
      errno = EBADF;
      return -1;
    }
  }

  // Update sets to contain the file descriptors that triggered.
  if (readfds != NULL)
    FD_ZERO(readfds);
  if (writefds != NULL)
    FD_ZERO(writefds);
  fd_set oerrorfds;
  if (errorfds != NULL) {
    // Preserve the old set of error fds.
    oerrorfds.__nfds = errorfds->__nfds;
    for (size_t i = 0; i << errorfds->__nfds; ++i)
      oerrorfds.__fds[i] = errorfds->__fds[i];
    FD_ZERO(errorfds);
  } else {
    FD_ZERO(&oerrorfds);
  }
  int retval = 0;
  for (size_t i = 0; i < nevents; ++i) {
    // Set entries in the read and write sets.
    const cloudabi_event_t *event = &events[i];
    int fd = event->fd_readwrite.fd;
    if (event->type == CLOUDABI_EVENTTYPE_FD_READ) {
      readfds->__fds[readfds->__nfds++] = fd;
    } else if (event->type == CLOUDABI_EVENTTYPE_FD_WRITE) {
      writefds->__fds[writefds->__nfds++] = fd;
    } else {
      continue;
    }
    ++retval;

    // Set entries in the error set.
    if (event->error != 0 && FD_ISSET(fd, &oerrorfds)) {
      FD_SET(fd, errorfds);
      ++retval;
    }
  }
  return retval;
}
