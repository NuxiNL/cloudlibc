// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <sys/procdesc.h>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>

int pdwait(int fd, siginfo_t *infop, int options) {
  if ((options & ~WNOHANG) != 0)
    return EINVAL;

  // Prepare set of events on which we should wait. If WNOHANG is
  // specified, add an additional zero-value clock to force poll() to
  // return immediately.
  cloudabi_subscription_t subscriptions[2] = {
      {
          .type = CLOUDABI_EVENTTYPE_PROC_TERMINATE, .proc_terminate.fd = fd,
      },
      {
          .type = CLOUDABI_EVENTTYPE_CLOCK,
          .clock.clock_id = CLOUDABI_CLOCK_MONOTONIC,
      },
  };

  // Wait for process termination.
  size_t triggered;
  cloudabi_event_t events[__arraycount(subscriptions)];
  cloudabi_errno_t error = cloudabi_sys_poll(
      subscriptions, events, (options & WNOHANG) != 0 ? 2 : 1, &triggered);
  if (error != 0)
    return error;

  for (size_t i = 0; i < triggered; ++i) {
    const cloudabi_event_t *ev = &events[i];
    if (ev->type == CLOUDABI_EVENTTYPE_PROC_TERMINATE) {
      if (ev->error != 0) {
        // Invalid file descriptor.
        return errno_fixup_process(fd, ev->error);
      }

      if (ev->proc_terminate.signal != 0) {
        // Process terminated due to a signal.
        *infop = (siginfo_t){
            .si_signo = SIGCHLD,
            .si_code = CLD_KILLED,
            .si_status = ev->proc_terminate.signal,
        };
      } else {
        // Process terminated through _Exit().
        *infop = (siginfo_t){
            .si_signo = SIGCHLD,
            .si_code = CLD_EXITED,
            .si_status = ev->proc_terminate.exitcode,
        };
      }
      return 0;
    }
  }

  // WNOHANG was provided, but the process is still running.
  *infop = (siginfo_t){};
  return 0;
}
