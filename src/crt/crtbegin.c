// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

// Beginning of section markers.
void (*__ctors_start[0])(void) __section(".ctors");
void (*__dtors_start[0])(void) __section(".dtors");
