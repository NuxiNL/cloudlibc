// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <time.h>

void tzset(void) {
  // Reset the contents of tzname to their default values.
  tzname[0] = (char *)"UTC";
  tzname[1] = (char *)"UTC";
}
