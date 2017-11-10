// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <errno.h>
#include <locale.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>

#include "syslog_impl.h"

static_assert(LOG_UPTO(LOG_EMERG) == LOG_MASK(LOG_EMERG),
              "LOG_UPTO(LOG_EMERG) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_ALERT) ==
                  (LOG_UPTO(LOG_EMERG) | LOG_MASK(LOG_ALERT)),
              "LOG_UPTO(LOG_ALERT) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_CRIT) == (LOG_UPTO(LOG_ALERT) | LOG_MASK(LOG_CRIT)),
              "LOG_UPTO(LOG_CRIT) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_ERR) == (LOG_UPTO(LOG_CRIT) | LOG_MASK(LOG_ERR)),
              "LOG_UPTO(LOG_ERR) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_WARNING) ==
                  (LOG_UPTO(LOG_ERR) | LOG_MASK(LOG_WARNING)),
              "LOG_UPTO(LOG_WARNING) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_NOTICE) ==
                  (LOG_UPTO(LOG_WARNING) | LOG_MASK(LOG_NOTICE)),
              "LOG_UPTO(LOG_NOTICE) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_INFO) == (LOG_UPTO(LOG_NOTICE) | LOG_MASK(LOG_INFO)),
              "LOG_UPTO(LOG_INFO) yields an incorrect mask");
static_assert(LOG_UPTO(LOG_DEBUG) == (LOG_UPTO(LOG_INFO) | LOG_MASK(LOG_DEBUG)),
              "LOG_UPTO(LOG_DEBUG) yields an incorrect mask");
static_assert((LOG_UPTO(LOG_WARNING) | LOG_MASK(LOG_DEBUG)) ==
                  (LOG_UPTO(LOG_DEBUG) &
                   ~(LOG_MASK(LOG_NOTICE) | LOG_MASK(LOG_INFO))),
              "Test vector provided by POSIX failed");

static const char messages[][10] = {
    [LOG_ALERT] = "ALERT    \0",  [LOG_CRIT] = "CRITICAL \0",
    [LOG_DEBUG] = "DEBUG    \0",  [LOG_EMERG] = "EMERGENCY\0",
    [LOG_ERR] = "ERROR    \0",    [LOG_INFO] = "INFO     \0",
    [LOG_NOTICE] = "NOTICE   \0", [LOG_WARNING] = "WARNING  \0",
};

void vsyslog_l(int priority, locale_t locale, const char *message, va_list ap) {
  // Save errno value, so vfprintf_l() uses the right value.
  int saved_errno = errno;

  // Validate priority and check whether it is part of the logging mask.
  if (priority < 0 || priority >= (int)__arraycount(messages))
    return;
  if ((atomic_load_explicit(&__syslog_logmask, memory_order_relaxed) &
       LOG_MASK(priority)) == 0)
    return;

  // Obtain the time of day.
  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    return;
  struct tm tm;
  if (gmtime_r(&ts.tv_sec, &tm) == NULL)
    return;

  // Print time of day, followed by the priority.
  flockfile(stderr);
  fprintf_l(stderr, locale, "%04d-%02d-%02dT%02d:%02d:%02d.%09ldZ %s ",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec, ts.tv_nsec, messages[priority]);

  // Print the error message.
  errno = saved_errno;
  vfprintf_l(stderr, locale, message, ap);

  // Add a trailing newline if the message itself does not end with one.
  size_t len = strlen(message);
  if (len == 0 || message[len - 1] != '\n')
    putc_unlocked('\n', stderr);
  funlockfile(stderr);
}
