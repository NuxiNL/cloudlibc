// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>

// As the POSIX functions for accessing the environment are not
// thread-safe, this implementation does not provide any routines to
// modify the contents of the environment. The environment simply
// consists of a fixed set of variables.
//
// By default the environment is empty. Entries can be added by adding
// them to the array below.
static const char *const envvars[] = {
    NULL,
};
extern const char *const *const environ;
const char *const *const environ = envvars;
