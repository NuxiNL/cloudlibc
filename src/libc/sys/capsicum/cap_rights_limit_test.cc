// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(cap_rights_limit, bad) {
  // Bad file descriptor.
  ASSERT_EQ(-1, cap_rights_limit(-1, NULL));
  ASSERT_EQ(EBADF, errno);
}

TEST(cap_rights_limit, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  int fd = openat(fd_tmp, "hello", O_CREAT | O_WRONLY);
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_ISREG(sb.st_mode));

  // Attempted to extend rights.
  cap_rights_t rights;
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_READ));
  ASSERT_EQ(-1, cap_rights_limit(fd, &rights));
  ASSERT_EQ(ENOTCAPABLE, errno);
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_ISREG(sb.st_mode));

  // Limiting rights is allowed.
  ASSERT_EQ(&rights, cap_rights_init(&rights, CAP_WRITE));
  ASSERT_EQ(0, cap_rights_limit(fd, &rights));

  // Validate rights. Call to fstat() should now fail.
  ASSERT_EQ(0, cap_rights_get(fd, &rights));
  // TODO(ed): This currently still grants some other rights on FreeBSD.
  // ASSERT_EQ(CAP_WRITE, rights.__value);
  ASSERT_EQ(-1, fstat(fd, &sb));
  ASSERT_EQ(ENOTCAPABLE, errno);

  ASSERT_EQ(0, close(fd));
}
