// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

// The global timezone in this environment is always UTC. Don't include
// <time.h> and redeclare the timezone variable as const, to ensure its
// contents are never overwritten.
extern const long timezone;
const long timezone = 0;
