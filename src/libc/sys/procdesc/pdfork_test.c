// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>
#include <sys/procdesc.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

TEST(pdfork, rights) {
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0)
    _Exit(200);

  // Validate file type.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISPROC(&sb));

  // Validate rights from a standard process descriptor.
  cap_rights_t base, inheriting;
  ASSERT_EQ(0, cap_rights_get_explicit(fd, &base, &inheriting));
  ASSERT_EQ(CAP_FSTAT | CAP_PDWAIT, base.__value);
  ASSERT_EQ(0, inheriting.__value);

  // Validate access mode. This should return O_SEARCH, as it is the
  // best we can do. It is not readable, not writable and not executable.
  ASSERT_EQ(O_SEARCH, fcntl(fd, F_GETFL));

  ASSERT_EQ(0, close(fd));
}
