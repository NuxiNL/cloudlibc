// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <argdata.h>
#include <program.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>

void program_main(const argdata_t *ad) {
  // Default configuration attributes.
  int tmpdir = -1;
  int logfile = -1;
  unsigned int nthreads = 1;

  // Fetch configuration attributes from argument data.
  argdata_map_iterator_t it;
  const argdata_t *key;
  const argdata_t *value;
  argdata_map_iterate(ad, &it);
  while (argdata_map_next(&it, &key, &value)) {
    // Fetch key.
    const char *keystr;
    if (argdata_get_str_c(key, &keystr) != 0)
      continue;

    // Set value depending on key.
    if (strcmp(keystr, "tmpdir") == 0)
      argdata_get_fd(value, &tmpdir);
    else if (strcmp(keystr, "logfile") == 0)
      argdata_get_fd(value, &logfile);
    else if (strcmp(keystr, "nthreads") == 0)
      argdata_get_int(value, &nthreads);
  }

  // Set up stderr.
  FILE *f = fdopen(logfile, "w");
  if (f != NULL)
    fswap(stderr, f);

  // Invoke test suite.
  testing_execute(tmpdir, logfile, nthreads);
  exit(0);
}
