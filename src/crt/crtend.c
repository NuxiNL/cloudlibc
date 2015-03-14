// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

// End of section markers.
void (*__ctors_stop[0])(void) __section(".ctors");
void (*__dtors_stop[0])(void) __section(".dtors");
