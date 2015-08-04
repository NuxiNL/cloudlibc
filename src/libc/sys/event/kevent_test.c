// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/event.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(kevent, ebadf) {
  // Invalid file descriptor.
  ASSERT_EQ(-1, kevent(-123, NULL, 0, NULL, 0, NULL));
  ASSERT_EQ(EBADF, errno);
}

TEST(kevent, null) {
  // Null operation on kqueue.
  int fd = kqueue();
  ASSERT_LE(0, fd);
  ASSERT_EQ(0, kevent(fd, NULL, 0, NULL, 0, NULL));
  ASSERT_EQ(0, close(fd));
}

TEST(kevent, evfilt_read_ebadf) {
  // Perform EVFILT_READ on a bad file descriptor.
  int fd = kqueue();
  ASSERT_LE(0, fd);
  struct kevent ke;
  EV_SET(&ke, -123, EVFILT_READ, EV_ADD, 0, 0xdeadc0de, (void *)0x42);
  ASSERT_EQ(1, kevent(fd, &ke, 1, &ke, 1, NULL));
  ASSERT_EQ(0, close(fd));

  // Validate resulting event.
  ASSERT_EQ(-123, (int)ke.ident);
  ASSERT_EQ(EVFILT_READ, ke.filter);
  ASSERT_EQ(EV_ERROR, ke.flags);
  ASSERT_EQ(0, ke.fflags);
  ASSERT_EQ(EBADF, ke.data);
  ASSERT_EQ((void *)0x42, ke.udata);
}

TEST(kevent, evfilt_read_pipe) {
  int fdq = kqueue();
  ASSERT_LE(0, fdq);
  int fdp[2];
  ASSERT_EQ(0, pipe(fdp));

  // Attempt to poll for reading on the writer side.
  {
    struct kevent ke;
    EV_SET(&ke, fdp[1], EVFILT_READ, EV_ADD, 0, 0xdeadc0de, &ke);
    ASSERT_EQ(1, kevent(fdq, &ke, 1, &ke, 1, NULL));
    ASSERT_EQ(fdp[1], (int)ke.ident);
    ASSERT_EQ(EVFILT_READ, ke.filter);
    ASSERT_EQ(EV_ERROR, ke.flags);
    ASSERT_EQ(0, ke.fflags);
    ASSERT_EQ(EINVAL, ke.data);
    ASSERT_EQ(&ke, ke.udata);
  }

  // Write data into the pipe. We should now be able to observe this data.
  ASSERT_EQ(5, write(fdp[1], "Hello", 5));
  {
    struct kevent ke;
    EV_SET(&ke, fdp[0], EVFILT_READ, EV_ADD, 0, 0xdeadc0de, (void *)0x42);
    ASSERT_EQ(1, kevent(fdq, &ke, 1, &ke, 1, NULL));
    ASSERT_EQ(fdp[0], (int)ke.ident);
    ASSERT_EQ(EVFILT_READ, ke.filter);
    ASSERT_EQ(0, ke.flags);
    ASSERT_EQ(0, ke.fflags);
    ASSERT_EQ(5, ke.data);
    ASSERT_EQ((void *)0x42, ke.udata);
  }

  // Close the writer side. EV_EOF should now be set.
  ASSERT_EQ(0, close(fdp[1]));
  {
    struct kevent ke;
    ASSERT_EQ(1, kevent(fdq, NULL, 0, &ke, 1, NULL));
    ASSERT_EQ(fdp[0], (int)ke.ident);
    ASSERT_EQ(EVFILT_READ, ke.filter);
    ASSERT_EQ(EV_EOF, ke.flags);
    ASSERT_EQ(0, ke.fflags);
    ASSERT_EQ(5, ke.data);
    ASSERT_EQ((void *)0x42, ke.udata);
  }

  // TODO(ed): Add more tests.

  ASSERT_EQ(0, close(fdq));
  ASSERT_EQ(0, close(fdp[0]));
}
