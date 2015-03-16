// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

int __cxa_atexit(void (*)(void *), void *, void *);

int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle) {
  // TODO(edje): Implement.
  return 0;
}
