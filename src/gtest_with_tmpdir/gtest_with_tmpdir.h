// Copyright (c) 2019 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef GTEST_WITH_TMPDIR_H
#define GTEST_WITH_TMPDIR_H

namespace gtest_with_tmpdir {

// CreateTemporaryDirectory creates an empty directory that can be used
// as part of a unit test. A file descriptor to the directory is
// returned, so that *at() system calls (e.g., openat(), mkdirat()) may
// be used to interact with the directory.
int CreateTemporaryDirectory();

}  // namespace gtest_with_tmpdir

#endif
