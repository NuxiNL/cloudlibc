// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <testing.h>

TEST(cap_rights_is_set, example) {
  // Empty rights set.
  cap_rights_t rights;
  ASSERT_EQ(&rights, cap_rights_init(&rights));

  // Read bit not set.
  ASSERT_FALSE(cap_rights_is_set(&rights, CAP_READ));

  // Set read bit and retry.
  ASSERT_EQ(&rights, cap_rights_set(&rights, CAP_READ));
  ASSERT_TRUE(cap_rights_is_set(&rights, CAP_READ));

  // Add some more bits.
  ASSERT_EQ(&rights,
            cap_rights_set(&rights, CAP_WRITE, CAP_FSTAT, CAP_UNLINKAT));
  ASSERT_TRUE(cap_rights_is_set(&rights, CAP_READ, CAP_WRITE));
  ASSERT_FALSE(cap_rights_is_set(&rights, CAP_READ, CAP_WRITE, CAP_CONNECT));
}
