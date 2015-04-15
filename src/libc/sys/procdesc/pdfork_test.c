// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>
#include <sys/procdesc.h>

#include <signal.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

TEST(pdfork, rights) {
  int fd;
  int ret = pdfork(&fd);
  if (ret == 0)
    _Exit(200);

  // Validate rights from a standard process descriptor.
  cap_rights_t base, inheriting;
  ASSERT_EQ(0, cap_rights_get_explicit(fd, &base, &inheriting));
  ASSERT_EQ(CAP_FSTAT | CAP_PDWAIT, base.__value);
  ASSERT_EQ(0, inheriting.__value);
  ASSERT_EQ(0, close(fd));
}
