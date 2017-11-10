// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>
#include <syslog.h>

#include "syslog_impl.h"

int setlogmask(int maskpri) {
  if (maskpri == 0) {
    // Return current logging mask.
    return atomic_load_explicit(&__syslog_logmask, memory_order_relaxed);
  } else {
    // Change logging mask and return the old value.
    return atomic_exchange_explicit(&__syslog_logmask, maskpri,
                                    memory_order_relaxed);
  }
}
