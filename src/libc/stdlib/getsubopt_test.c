// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <testing.h>

TEST(getsubopt, example) {
  char *keylist[] = {(char *)"ro", (char *)"rw", (char *)"port", NULL};
  char option[] = "rw,port=1234,bad";
  char *optionp = option;
  char *valuep;

  ASSERT_EQ(1, getsubopt(&optionp, keylist, &valuep));
  ASSERT_ARREQ("rw\0port=1234,bad", option, __arraycount(option));
  ASSERT_EQ(option + 3, optionp);
  ASSERT_EQ(NULL, valuep);

  ASSERT_EQ(2, getsubopt(&optionp, keylist, &valuep));
  ASSERT_ARREQ("rw\0port=1234\0bad", option, __arraycount(option));
  ASSERT_EQ(option + 13, optionp);
  ASSERT_STREQ("1234", valuep);

  ASSERT_EQ(-1, getsubopt(&optionp, keylist, &valuep));
  ASSERT_EQ(&option[13], optionp);
}
