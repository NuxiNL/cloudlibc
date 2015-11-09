// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

// The global timezone in this environment is always UTC. Don't include
// <time.h> and redeclare the tzname array as const, to ensure its
// contents are never overwritten.
extern const char *const tzname[];
const char *const tzname[2] = {"UTC", "UTC"};
