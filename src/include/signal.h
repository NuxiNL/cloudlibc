// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <signal.h> - signals
//
// Extensions:
// - NSIG:
//   Present on most operating systems.
// - psignal_l():
//   psignal() always uses the C locale.
//
// Features missing:
// - pid_t, siginfo_t::si_pid, kill() and killpg():
//   Requires global process namespace.
// - siginfo_t::si_uid and uid_t:
//   POSIX credential management is not available.
// - Everything else:
//   Installing signal handlers is not supported.

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <_/struct/timespec.h>
#include <_/types.h>

#ifndef _PTHREAD_ATTR_T_DECLARED
typedef __pthread_attr_t pthread_attr_t;
#define _PTHREAD_ATTR_T_DECLARED
#endif
#ifndef _PTHREAD_T_DECLARED
typedef __pthread_t pthread_t;
#define _PTHREAD_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif

// Signals.
#define SIGABRT 1     // Process abort signal.
#define SIGALRM 2     // Alarm clock.
#define SIGBUS 3      // Access to an undefined portion of a memory object.
#define SIGCHLD 4     // Child process terminated, stopped or continued.
#define SIGCONT 5     // Continue executing, if stopped.
#define SIGFPE 6      // Erroneous arithmetic operation.
#define SIGHUP 7      // Hangup.
#define SIGILL 8      // Illegal instruction.
#define SIGINT 9      // Terminal interrupt signal.
#define SIGKILL 10    // Kill (cannot be caught or ignored).
#define SIGPIPE 11    // Write on a pipe with no one to read it.
#define SIGQUIT 12    // Terminal quit signal.
#define SIGSEGV 13    // Invalid memory reference.
#define SIGSTOP 14    // Stop executing (cannot be caught or ignored).
#define SIGSYS 15     // Termination signal.
#define SIGTERM 16    // Terminal stop signal.
#define SIGTRAP 17    // Trace/breakpoint trap.
#define SIGTSTP 18    // Terminal stop signal.
#define SIGTTIN 19    // Background process attempting read.
#define SIGTTOU 20    // Background process attempting write.
#define SIGURG 21     // High bandwidth data is available at a socket.
#define SIGUSR1 22    // User-defined signal 1.
#define SIGUSR2 23    // User-defined signal 2.
#define SIGVTALRM 24  // Virtual timer expired.
#define SIGXCPU 25    // CPU time limit exceeded.
#define SIGXFSZ 26    // File size limit exceeded.

#define NSIG 27  // Last signal plus one.

// Signal codes returned by pdwait().
#define CLD_KILLED 1  // Child has terminated abnormally.
#define CLD_EXITED 2  // Child has exited.

struct __siginfo {
  int si_signo;   // Signal number.
  int si_code;    // Signal code.
  int si_status;  // Exit value or signal.
};
typedef __siginfo_t siginfo_t;

__BEGIN_DECLS
void psignal(int, const char *);
void psignal_l(int, const char *, __locale_t);
int raise(int);
__END_DECLS

#endif
