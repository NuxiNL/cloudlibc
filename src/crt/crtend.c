// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>

// End of section markers.
void (*__ctors_stop[0])(void) __section(".ctors");
void (*__dtors_stop[0])(void) __section(".dtors");
