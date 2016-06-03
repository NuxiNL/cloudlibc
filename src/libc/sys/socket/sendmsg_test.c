// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>
#include <sys/un.h>

#include <errno.h>
#include <fcntl.h>
#include <stdalign.h>
#include <testing.h>
#include <unistd.h>

TEST(sendmsg, bad) {
  // Bad file descriptor.
  char b = 'A';
  struct iovec biov = {.iov_base = &b, .iov_len = 1};
  struct msghdr message = {.msg_iov = &biov, .msg_iovlen = 1};
  ASSERT_EQ(-1, sendmsg(-1, &message, 0));
  ASSERT_EQ(EBADF, errno);

  // Number of I/O vectors is not positive.
  ASSERT_EQ(-1, sendmsg(-1, &(struct msghdr){.msg_iovlen = 0}, 0));
  ASSERT_EQ(EMSGSIZE, errno);
  ASSERT_EQ(-1, sendmsg(-1, &(struct msghdr){.msg_iovlen = -42}, 0));
  ASSERT_EQ(EMSGSIZE, errno);

  // Not connected.
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);
  ASSERT_EQ(-1, sendmsg(fd, &message, 0));
  ASSERT_EQ(ENOTCONN, errno);
  ASSERT_EQ(0, close(fd));

  // Not a socket.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));
  ASSERT_EQ(-1, sendmsg(fds[0], &message, 0));
  ASSERT_EQ(ENOTSOCK, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));

  // Bad flags.
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(-1, sendmsg(fds[0], &message, 0xdeadc0de));
  ASSERT_EQ(EOPNOTSUPP, errno);

  // Destination address cannot be specified in this implementation.
  struct sockaddr_un sun = {.sun_family = AF_UNIX};
  message.msg_name = &sun;
  message.msg_namelen = sizeof(sun);
  ASSERT_EQ(-1, sendmsg(fds[0], &message, 0));
  ASSERT_EQ(ENOTCAPABLE, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST_SEPARATE_PROCESS(sendmsg, epipe) {
  // Connection has been closed.
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(0, close(fds[1]));

  char b = 'A';
  struct iovec biov = {.iov_base = &b, .iov_len = 1};
  struct msghdr message = {.msg_iov = &biov, .msg_iovlen = 1};
  ASSERT_EQ(-1, sendmsg(fds[0], &message, 0));
  ASSERT_EQ(EPIPE, errno);
  ASSERT_EQ(0, close(fds[0]));
}

TEST(sendmsg, example) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));

  // Send a message through one side of the socket pair.
  {
    struct iovec iov[2] = {{.iov_base = (void *)"Hello ", .iov_len = 6},
                           {.iov_base = (void *)"world!", .iov_len = 6}};
    alignas(struct cmsghdr) char control[CMSG_SPACE(2 * sizeof(int))];
    struct msghdr message = {
        .msg_iov = iov,
        .msg_iovlen = __arraycount(iov),
        .msg_control = control,
        .msg_controllen = sizeof(control),
        .msg_flags = 0xdeadc0de,
    };

    // Send both ends of a pipe across.
    int pipefds[2];
    ASSERT_EQ(0, pipe(pipefds));

    // Place file descriptors in ancillary data.
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&message);
    ASSERT_NE(NULL, cmsg);
    cmsg->cmsg_len = CMSG_LEN(2 * sizeof(int));
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    int *fdbuf = (int *)CMSG_DATA(cmsg);
    fdbuf[0] = pipefds[0];
    fdbuf[1] = pipefds[1];

    ASSERT_EQ(12, sendmsg(fds[0], &message, 0));
    ASSERT_EQ(0, close(pipefds[0]));
    ASSERT_EQ(0, close(pipefds[1]));
  }

  // Receive message through the other side.
  {
    char strings[3][4];
    struct iovec iov[3] = {
        {.iov_base = strings[0], .iov_len = sizeof(strings[0])},
        {.iov_base = strings[1], .iov_len = sizeof(strings[1])},
        {.iov_base = strings[2], .iov_len = sizeof(strings[2])}};
    struct msghdr message = {
        .msg_iov = iov, .msg_iovlen = __arraycount(iov),
    };

    ASSERT_EQ(12, recvmsg(fds[1], &message, 0));
    ASSERT_EQ(0, message.msg_flags);
    ASSERT_ARREQ("Hell", strings[0], 4);
    ASSERT_ARREQ("o wo", strings[1], 4);
    ASSERT_ARREQ("rld!", strings[2], 4);

    // TODO(ed): Test sent file descriptors!
  }

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}
