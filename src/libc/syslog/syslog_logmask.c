// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <syslog.h>

#include "syslog_impl.h"

// Logging mask as controlled by setlogmask(). The default log mask
// allows all priorities to be logged.
atomic_int __syslog_logmask = ATOMIC_VAR_INIT(LOG_UPTO(LOG_DEBUG));
