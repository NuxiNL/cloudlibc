// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/event.h>

#include <errno.h>

int kevent(int fd, const struct kevent *in, int nin, struct kevent *out,
           int nout, const struct timespec *timeout) {
  if (nin < 0 || nout < 0) {
    errno = EINVAL;
    return -1;
  }

  size_t nevents = nin > nout ? nin : nout;
  cloudabi_event_t events[nevents];
  for (int i = 0; i < nin; ++i) {
    const struct kevent *ke = &in[i];
    cloudabi_event_t *ev = &events[i];
    ev->userdata = (uintptr_t)ke->udata;
    // TODO(edje): Translate flags and fflags.
    switch (ke->filter) {
      case EVFILT_READ:
        ev->type = CLOUDABI_EVENT_TYPE_FD_READ;
        ev->fd_readwrite.fd = ke->ident;
        break;
      case EVFILT_WRITE:
        ev->type = CLOUDABI_EVENT_TYPE_FD_WRITE;
        ev->fd_readwrite.fd = ke->ident;
        break;
      default:
        errno = EINVAL;
        return -1;
    }
  }

  // TODO(edje): Add timeout.

  size_t ntriggered;
  cloudabi_errno_t error =
      cloudabi_sys_poll(fd, events, nin, events, nout, &ntriggered);
  if (error != 0) {
    errno = error;
    return -1;
  }

  // TODO(edje): Convert results back to struct kevent.
  errno = ENOSYS;
  return -1;
}
