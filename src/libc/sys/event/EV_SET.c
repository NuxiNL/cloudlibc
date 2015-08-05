// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/event.h>

#include <stdint.h>

#ifndef EV_SET
#error "EV_SET is supposed to be a macro as well"
#endif

// clang-format off
void (EV_SET)(struct kevent *ke, uintptr_t ident, short filter,
              unsigned short flags, unsigned int fflags, intptr_t data,
              void *udata) {
  EV_SET(ke, ident, filter, flags, fflags, data, udata);
}
