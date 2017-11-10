// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>

// Beginning of section markers.
void (*__ctors_start[0])(void) __section(".ctors");
void (*__dtors_start[0])(void) __section(".dtors");
