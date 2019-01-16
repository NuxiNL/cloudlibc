// Copyright (c) 2019 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

#include <fcntl.h>
#include <program.h>
#include <stdio.h>
#include <unistd.h>
#include <argdata.hpp>
#include <atomic>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {
std::atomic<int> next_directory_id;
std::optional<int> temporary_directory_fd;
}  // namespace

namespace gtest_with_tmpdir {

int CreateTemporaryDirectory() {
  // Allocate new name for subdirectory.
  auto subdirectory_name =
      std::to_string(next_directory_id.fetch_add(1, std::memory_order_relaxed));

  // Create subdirectory.
  EXPECT_TRUE(temporary_directory_fd);
  EXPECT_EQ(0,
            mkdirat(*temporary_directory_fd, subdirectory_name.c_str(), 0700));

  // Create file descriptor to it, to be used by the test.
  int subdirectory_fd =
      openat(*temporary_directory_fd, subdirectory_name.c_str(),
             O_DIRECTORY | O_RDONLY);
  EXPECT_LE(0, subdirectory_fd);
  return subdirectory_fd;
}

}  // namespace gtest_with_tmpdir

void program_main(const argdata_t *ad) {
  // Fetch configuration attributes from argument data.
  std::optional<int> logfile;
  for (auto [key, value] : ad->as_map()) {
    if (std::optional<std::string_view> keystr = key->get_str(); keystr) {
      if (keystr == "logfile")
        logfile = value->get_fd();
      else if (keystr == "tmpdir")
        temporary_directory_fd = value->get_fd();
    }
  }

  // Set up stderr.
  if (logfile) {
    FILE *f = fdopen(*logfile, "w");
    if (f != NULL)
      fswap(stderr, f);
  }

  // Call into regular initialization.
  int argc = 1;
  char *argv[] = {const_cast<char *>("test"), nullptr};
  testing::InitGoogleMock(&argc, argv);
  std::exit(RUN_ALL_TESTS());
}
