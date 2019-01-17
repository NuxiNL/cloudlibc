// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <iterator>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(sendmsg, bad) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // Bad file descriptor.
  char b = 'A';
  struct iovec biov = {.iov_base = &b, .iov_len = 1};
  struct msghdr message = {.msg_iov = &biov, .msg_iovlen = 1};
  ASSERT_EQ(-1, sendmsg(-1, &message, 0));
  ASSERT_EQ(EBADF, errno);

  // Number of I/O vectors is not positive.
  {
    struct msghdr msg = {.msg_iovlen = 0};
    ASSERT_EQ(-1, sendmsg(-1, &msg, 0));
    ASSERT_EQ(EMSGSIZE, errno);
  }
  {
    struct msghdr msg = {.msg_iovlen = -42};
    ASSERT_EQ(-1, sendmsg(-1, &msg, 0));
    ASSERT_EQ(EMSGSIZE, errno);
  }

  // Not a socket.
  {
    ASSERT_EQ(-1, sendmsg(fd_tmp, &message, 0));
    ASSERT_EQ(ENOTSOCK, errno);
  }

  // Bad flags.
  {
    int fds[2];
    ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
    ASSERT_EQ(-1, sendmsg(fds[0], &message, 0xdeadc0de));
    ASSERT_EQ(EOPNOTSUPP, errno);
    ASSERT_EQ(0, close(fds[0]));
    ASSERT_EQ(0, close(fds[1]));
  }
}

TEST(sendmsg, epipe) {
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
        .msg_iovlen = std::size(iov),
        .msg_control = control,
        .msg_controllen = sizeof(control),
        .msg_flags = 0x1badc0de,
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
    char strings[3][5];
    struct iovec iov[3] = {
        {.iov_base = strings[0], .iov_len = sizeof(strings[0]) - 1},
        {.iov_base = strings[1], .iov_len = sizeof(strings[1]) - 1},
        {.iov_base = strings[2], .iov_len = sizeof(strings[2]) - 1}};
    struct msghdr message = {
        .msg_iov = iov,
        .msg_iovlen = std::size(iov),
    };

    ASSERT_EQ(12, recvmsg(fds[1], &message, 0));
    ASSERT_EQ(0, message.msg_flags);
    strings[0][4] = '\0';
    ASSERT_STREQ("Hell", strings[0]);
    strings[1][4] = '\0';
    ASSERT_STREQ("o wo", strings[1]);
    strings[2][4] = '\0';
    ASSERT_STREQ("rld!", strings[2]);

    // TODO(ed): Test sent file descriptors!
  }

  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(sendmsg, fd_passing) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int sfds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, sfds));

  // Attempt to send a bad file descriptor through a UNIX socket, using
  // SCM_RIGHTS. This should make the call fail entirely with EBADF.
  {
    int badfd = -1;
    alignas(struct cmsghdr) char cmsgbuf[CMSG_SPACE(sizeof(badfd))];
    struct iovec iov = {.iov_base = (void *)"Hello", .iov_len = 5};
    struct msghdr msghdr = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = cmsgbuf,
        .msg_controllen = sizeof(cmsgbuf),
    };
    {
      struct cmsghdr *cmsghdr = CMSG_FIRSTHDR(&msghdr);
      cmsghdr->cmsg_len = CMSG_LEN(sizeof(badfd));
      cmsghdr->cmsg_level = SOL_SOCKET;
      cmsghdr->cmsg_type = SCM_RIGHTS;
      memcpy(CMSG_DATA(cmsghdr), &badfd, sizeof(badfd));
    }
    ASSERT_EQ(-1, sendmsg(sfds[0], &msghdr, 0));
    ASSERT_EQ(EBADF, errno);
  }

  // Send two file descriptors using a control message across a UNIX
  // socket, using SCM_RIGHTS.
  {
    int pfds[2];
    pfds[0] = fd_tmp;
    pfds[1] = openat(fd_tmp, "file", O_CREAT | O_WRONLY);
    alignas(struct cmsghdr) char cmsgbuf[CMSG_SPACE(sizeof(pfds))];
    struct msghdr msghdr = {
        .msg_control = cmsgbuf,
        .msg_controllen = sizeof(cmsgbuf),
    };
    {
      struct cmsghdr *cmsghdr = CMSG_FIRSTHDR(&msghdr);
      cmsghdr->cmsg_len = CMSG_LEN(sizeof(pfds));
      cmsghdr->cmsg_level = SOL_SOCKET;
      cmsghdr->cmsg_type = SCM_RIGHTS;
      memcpy(CMSG_DATA(cmsghdr), pfds, sizeof(pfds));
    }

    // We can't send zero bytes of data, even if we're attaching file
    // descriptors to this message.
    ASSERT_EQ(-1, sendmsg(sfds[0], &msghdr, 0));
    ASSERT_EQ(EMSGSIZE, errno);

    // Transmission should succeed once data has been attached.
    struct iovec iov = {.iov_base = (void *)"Hello", .iov_len = 5};
    msghdr.msg_iov = &iov;
    msghdr.msg_iovlen = 1;
    ASSERT_EQ(5, sendmsg(sfds[0], &msghdr, 0));
    ASSERT_EQ(0, close(pfds[1]));
  }

  // Receive data and file descriptors.
  {
    char buf[6];
    struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
    alignas(struct cmsghdr) char cmsgbuf[CMSG_SPACE(3 * sizeof(int))];
    struct msghdr msghdr = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
        .msg_control = cmsgbuf,
        .msg_controllen = sizeof(cmsgbuf),
    };

    ASSERT_EQ(5, recvmsg(sfds[1], &msghdr, 0));
    buf[5] = '\0';
    ASSERT_STREQ("Hello", buf);
    {
      struct cmsghdr *cmsghdr = CMSG_FIRSTHDR(&msghdr);
      ASSERT_NE(NULL, cmsghdr);
      int pfds[2];
      ASSERT_EQ(CMSG_LEN(sizeof(pfds)), cmsghdr->cmsg_len);
      ASSERT_EQ(SOL_SOCKET, cmsghdr->cmsg_level);
      ASSERT_EQ(SCM_RIGHTS, cmsghdr->cmsg_type);
      ASSERT_EQ(NULL, CMSG_NXTHDR(&msghdr, cmsghdr));
      memcpy(pfds, CMSG_DATA(cmsghdr), sizeof(pfds));

      // Validate and close received file descriptors.
      struct stat sb;
      ASSERT_EQ(0, fstat(pfds[0], &sb));
      ASSERT_TRUE(S_ISDIR(sb.st_mode));
      ASSERT_EQ(0, close(pfds[0]));
      ASSERT_EQ(0, fstat(pfds[1], &sb));
      ASSERT_TRUE(S_ISREG(sb.st_mode));
      ASSERT_EQ(0, close(pfds[1]));
    }
  }

  ASSERT_EQ(0, close(sfds[0]));
  ASSERT_EQ(0, close(sfds[1]));
}
