// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/event.h>

#include <errno.h>

static_assert(EV_ADD == CLOUDABI_SUBSCRIPTION_ADD, "Value mismatch");
static_assert(EV_CLEAR == CLOUDABI_SUBSCRIPTION_CLEAR, "Value mismatch");
static_assert(EV_DELETE == CLOUDABI_SUBSCRIPTION_DELETE, "Value mismatch");
static_assert(EV_DISABLE == CLOUDABI_SUBSCRIPTION_DISABLE, "Value mismatch");
static_assert(EV_ENABLE == CLOUDABI_SUBSCRIPTION_ENABLE, "Value mismatch");
static_assert(EV_ONESHOT == CLOUDABI_SUBSCRIPTION_ONESHOT, "Value mismatch");

int kevent(int fd, const struct kevent *in, int nin, struct kevent *out,
           int nout, const struct timespec *timeout) {
  if (nin < 0 || nout < 0) {
    errno = EINVAL;
    return -1;
  }

  cloudabi_subscription_t subscriptions[nin];
  for (int i = 0; i < nin; ++i) {
    const struct kevent *ke = &in[i];
    cloudabi_subscription_t *sub = &subscriptions[i];
    sub->userdata = (uintptr_t)ke->udata;
    // TODO(edje): Translate flags and fflags.
    switch (ke->filter) {
      case EVFILT_READ:
        sub->type = CLOUDABI_EVENTTYPE_FD_READ;
        sub->fd_readwrite.fd = ke->ident;
        break;
      case EVFILT_WRITE:
        sub->type = CLOUDABI_EVENTTYPE_FD_WRITE;
        sub->fd_readwrite.fd = ke->ident;
        break;
      default:
        errno = EINVAL;
        return -1;
    }
  }

  // TODO(edje): Add timeout.

  size_t ntriggered;
  cloudabi_event_t events[nout];
  cloudabi_errno_t error =
      cloudabi_sys_poll(fd, subscriptions, nin, events, nout, &ntriggered);
  if (error != 0) {
    errno = error;
    return -1;
  }

  // TODO(edje): Convert results back to struct kevent.
  errno = ENOSYS;
  return -1;
}
