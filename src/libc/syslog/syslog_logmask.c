// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <syslog.h>

#include "syslog_impl.h"

// Logging mask as controlled by setlogmask(). The default log mask
// allows all priorities to be logged.
atomic_int __syslog_logmask = ATOMIC_VAR_INIT(
    LOG_MASK(LOG_DEBUG) | LOG_MASK(LOG_INFO) | LOG_MASK(LOG_NOTICE) |
    LOG_MASK(LOG_WARNING) | LOG_MASK(LOG_ERR) | LOG_MASK(LOG_CRIT) |
    LOG_MASK(LOG_ALERT) | LOG_MASK(LOG_EMERG));
