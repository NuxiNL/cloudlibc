// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <program.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <testing.h>
#include <unistd.h>

static int tmpdir = -1;

static bool iterate(const argdata_t *ad, void *thunk) {
  if (tmpdir < 0) {
    // First element in sequence: temporary directory for test data.
    if (argdata_get_fd(ad, &tmpdir) != 0)
      exit(1);
    return true;
  } else {
    // Second element in sequence; log file.
    int logfile;
    if (argdata_get_fd(ad, &logfile) != 0)
      exit(1);

    // Execute the unit tests.
    fswap(stderr, fdopen(logfile, "w"));
    testing_execute(tmpdir, logfile);
    exit(0);
  }
}

void program_main(const argdata_t *ad) {
  argdata_iterate_seq(ad, NULL, iterate);
  exit(1);
}
