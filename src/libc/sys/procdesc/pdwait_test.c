// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/procdesc.h>

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

TEST(pdwait, bad) {
  // Bad file descriptor.
  ASSERT_EQ(EBADF, pdwait(0xdeadc0de, NULL, 0));
  ASSERT_EQ(EBADF, pdwait(0xdeadc0de, NULL, WNOHANG));

  // Invalid file descriptor type.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(EINVAL, pdwait(fds[0], NULL, 0));
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Invalid flags.
  ASSERT_EQ(EINVAL, pdwait(fd_tmp, NULL, 0xdeadc0de));
}

TEST(pdwait, cld_exited) {
  // Pick a random exit code that we should expect.
  // TODO(ed): FreeBSD and NetBSD still truncate the exit code to 8
  // bits. We should propagate the full integer, as discussed here:
  // http://austingroupbugs.net/view.php?id=594
  uint8_t exitcode;
  arc4random_buf(&exitcode, sizeof(exitcode));

  // Fork process.
  int fd;
  int ret = pdfork(&fd);

  // Terminate the child process.
  if (ret == 0)
    _Exit(exitcode);

  // Validate exit code.
  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(exitcode, si.si_status);

  // Attempt to obtain exit code twice.
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_EXITED, si.si_code);
  ASSERT_EQ(exitcode, si.si_status);
  ASSERT_EQ(0, close(fd));
}

TEST(pdwait, cld_killed) {
  // Fork process.
  int fd;
  int ret = pdfork(&fd);

  // Terminate the child process.
  if (ret == 0) {
    raise(SIGSEGV);
    _Exit(200);
  }

  // Validate signal.
  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_KILLED, si.si_code);
  ASSERT_EQ(SIGSEGV, si.si_status);

  // Attempt to obtain exit code twice.
  ASSERT_EQ(0, pdwait(fd, &si, 0));
  ASSERT_EQ(SIGCHLD, si.si_signo);
  ASSERT_EQ(CLD_KILLED, si.si_code);
  ASSERT_EQ(SIGSEGV, si.si_status);
  ASSERT_EQ(0, close(fd));
}

TEST(pdwait, wnohang) {
  // Fork process.
  int fd;
  int ret = pdfork(&fd);

  // Wait a bit before terminating.
  if (ret == 0) {
    sleep(1);
    _Exit(12);
  }

  // pdwait() should not return any info for the child process.
  ASSERT_LT(0, ret);
  siginfo_t si;
  ASSERT_EQ(0, pdwait(fd, &si, WNOHANG));
  ASSERT_EQ(0, si.si_signo);
  ASSERT_EQ(0, close(fd));
}
