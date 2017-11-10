// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <errno.h>
#include <signal.h>

static_assert(SIGABRT == CLOUDABI_SIGABRT, "Value mismatch");
static_assert(SIGALRM == CLOUDABI_SIGALRM, "Value mismatch");
static_assert(SIGBUS == CLOUDABI_SIGBUS, "Value mismatch");
static_assert(SIGCHLD == CLOUDABI_SIGCHLD, "Value mismatch");
static_assert(SIGCONT == CLOUDABI_SIGCONT, "Value mismatch");
static_assert(SIGFPE == CLOUDABI_SIGFPE, "Value mismatch");
static_assert(SIGHUP == CLOUDABI_SIGHUP, "Value mismatch");
static_assert(SIGILL == CLOUDABI_SIGILL, "Value mismatch");
static_assert(SIGINT == CLOUDABI_SIGINT, "Value mismatch");
static_assert(SIGKILL == CLOUDABI_SIGKILL, "Value mismatch");
static_assert(SIGPIPE == CLOUDABI_SIGPIPE, "Value mismatch");
static_assert(SIGQUIT == CLOUDABI_SIGQUIT, "Value mismatch");
static_assert(SIGSEGV == CLOUDABI_SIGSEGV, "Value mismatch");
static_assert(SIGSTOP == CLOUDABI_SIGSTOP, "Value mismatch");
static_assert(SIGSYS == CLOUDABI_SIGSYS, "Value mismatch");
static_assert(SIGTERM == CLOUDABI_SIGTERM, "Value mismatch");
static_assert(SIGTRAP == CLOUDABI_SIGTRAP, "Value mismatch");
static_assert(SIGTSTP == CLOUDABI_SIGTSTP, "Value mismatch");
static_assert(SIGTTIN == CLOUDABI_SIGTTIN, "Value mismatch");
static_assert(SIGTTOU == CLOUDABI_SIGTTOU, "Value mismatch");
static_assert(SIGURG == CLOUDABI_SIGURG, "Value mismatch");
static_assert(SIGUSR1 == CLOUDABI_SIGUSR1, "Value mismatch");
static_assert(SIGUSR2 == CLOUDABI_SIGUSR2, "Value mismatch");
static_assert(SIGVTALRM == CLOUDABI_SIGVTALRM, "Value mismatch");
static_assert(SIGXCPU == CLOUDABI_SIGXCPU, "Value mismatch");
static_assert(SIGXFSZ == CLOUDABI_SIGXFSZ, "Value mismatch");

int raise(int sig) {
  // raise(0) always succeeds.
  if (sig == 0)
    return 0;

  // Bounds checking on the signal.
  if (sig < 1 || sig >= NSIG) {
    errno = EINVAL;
    return -1;
  }

  // Send signal to this process.
  cloudabi_errno_t error = cloudabi_sys_proc_raise(sig);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return 0;
}
