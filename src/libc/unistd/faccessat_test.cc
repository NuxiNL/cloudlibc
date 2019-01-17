// Copyright (c) 2016-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>
#include <sys/socket.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(faccessat, eacces) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  cap_rights_t r;
  ASSERT_EQ(0, cap_rights_get(fd_tmp, &r));
  cap_rights_clear(&r, CAP_WRITE);
  // TODO(ed): FreeBSD still grants CAP_WRITE if CAP_POSIX_FALLOCATE is
  // set. Removing this shouldn't be needed.
  cap_rights_clear(&r, CAP_POSIX_FALLOCATE);
  ASSERT_EQ(0, cap_rights_limit(fd_tmp, &r));
  ASSERT_EQ(-1, faccessat(fd_tmp, ".", W_OK, 0));
  ASSERT_EQ(EACCES, errno);
}

TEST(faccessat, ebadf) {
  ASSERT_EQ(-1, faccessat(-123, "hello", R_OK, 0));
  ASSERT_EQ(EBADF, errno);
}

TEST(faccessat, einval) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(-1, faccessat(fd_tmp, ".", 0xdeadc0de, 0));
  ASSERT_EQ(EINVAL, errno);
  ASSERT_EQ(-1, faccessat(fd_tmp, ".", R_OK, 0xdeadc0de));
  ASSERT_EQ(EINVAL, errno);
}

TEST(faccessat, eloop) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(0, symlinkat("hello", fd_tmp, "hello"));
  ASSERT_EQ(-1, faccessat(fd_tmp, "hello", F_OK, AT_EACCESS));
}

TEST(faccessat, enoent) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(-1, faccessat(fd_tmp, "hello", R_OK, 0));
  ASSERT_EQ(ENOENT, errno);
}

TEST(faccessat, enotdir) {
  int fds[2];
  ASSERT_EQ(0, socketpair(AF_UNIX, SOCK_STREAM, 0, fds));
  ASSERT_EQ(-1, faccessat(fds[0], "hello", R_OK, 0));
  ASSERT_EQ(ENOTDIR, errno);
  ASSERT_EQ(0, close(fds[0]));
  ASSERT_EQ(0, close(fds[1]));
}

TEST(faccessat, ok) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  ASSERT_EQ(0, faccessat(fd_tmp, ".", R_OK | X_OK, 0));
}
