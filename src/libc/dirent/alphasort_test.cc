// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

static struct dirent *create_dirent(const char *name) {
  size_t len = strlen(name) + 1;
  auto de = static_cast<struct dirent *>(
      malloc(offsetof(struct dirent, d_name) + len));
  memcpy(de->d_name, name, len);
  return de;
}

TEST(alphasort, example) {
  struct dirent *a = create_dirent("Hello");
  const struct dirent *pa = a;
  struct dirent *b = create_dirent("World");
  const struct dirent *pb = b;
  ASSERT_EQ(0, alphasort(&pa, &pa));
  ASSERT_GT(0, alphasort(&pa, &pb));
  ASSERT_LT(0, alphasort(&pb, &pa));
  ASSERT_EQ(0, alphasort(&pb, &pb));
  free(a);
  free(b);
}
