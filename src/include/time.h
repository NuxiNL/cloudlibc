// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <time.h> - time types
//
// Extensions:
// - localtime_l(), mktime_l():
//   localtime() and mktime() always use UTC. localtime_l() and
//   mktime_l() also provide sub-second granularity.
// - strptime_l():
//   strptime() always uses the C locale.
// - struct tm::tm_gmtoff and struct tm::tm_zone:
//   Timezone information used by strftime(). Present on many other
//   systems.
// - struct tm::tm_nsec:
//   Adds sub-second precision to strftime()/strptime().
// - nanosleep() and clock_nanosleep():
//   Last argument is optional, as calls can never be interrupted.
// - timegm():
//   mktime() for UTC. Present on many other systems.
//
// Features missing:
// - clock_getcpuclockid():
//   Requires global process namespace.
// - clock_settime():
//   Requires administrative privileges.
// - asctime(), ctime(), gmtime() and localtime():
//   Not thread-safe.
// - struct itimerspec, timer_create(), timer_delete(), timer_getoverrun(),
//   timer_gettime(), timer_settime():
//   Requires signal handling support.

#ifndef _TIME_H_
#define _TIME_H_

#include <_/limits.h>
#include <_/struct/timespec.h>  // IWYU pragma: export
#include <_/types.h>

#ifndef _CLOCK_T_DECLARED
typedef __clock_t clock_t;
#define _CLOCK_T_DECLARED
#endif
#ifndef _CLOCKID_T_DECLARED
typedef __clockid_t clockid_t;
#define _CLOCKID_T_DECLARED
#endif
#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _TIME_T_DECLARED
typedef __time_t time_t;
#define _TIME_T_DECLARED
#endif

struct tm {
  // Standard fields.
  int tm_sec;    // Seconds [0,59].
  int tm_min;    // Minutes [0,59].
  int tm_hour;   // Hour [0,23].
  int tm_mday;   // Day of month [1,31].
  int tm_mon;    // Month of year [0,11].
  int tm_year;   // Years since 1900.
  int tm_wday;   // Day of week [0,6] (Sunday = 0).
  int tm_yday;   // Day of year [0,365].
  int tm_isdst;  // Daylight Savings flag.

  // Extensions.
  int tm_gmtoff;        // Offset from UTC in seconds.
  const char *tm_zone;  // Timezone abbreviation.
  long tm_nsec;         // Nanoseconds [0,999999999].
};

#define NULL _NULL

#define CLOCKS_PER_SEC _UINT64_C(1000000000)
_Static_assert(_Generic(CLOCKS_PER_SEC, clock_t : 1),
               "CLOCKS_PER_SEC has to be of type clock_t");

extern const struct __clockid _CLOCK_MONOTONIC;
#define CLOCK_MONOTONIC (&_CLOCK_MONOTONIC)
extern const struct __clockid _CLOCK_PROCESS_CPUTIME_ID;
#define CLOCK_PROCESS_CPUTIME_ID (&_CLOCK_PROCESS_CPUTIME_ID)
extern const struct __clockid _CLOCK_REALTIME;
#define CLOCK_REALTIME (&_CLOCK_REALTIME)
extern const struct __clockid _CLOCK_THREAD_CPUTIME_ID;
#define CLOCK_THREAD_CPUTIME_ID (&_CLOCK_THREAD_CPUTIME_ID)

#define TIMER_ABSTIME 1

#define TIME_UTC 3

extern int daylight;
extern long timezone;
extern char *tzname[];

__BEGIN_DECLS
char *asctime_r(const struct tm *__restrict, char *__restrict);
clock_t clock(void);
int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, ...);
char *ctime_r(const time_t *, char *);
double difftime(time_t, time_t);
struct tm *gmtime_r(const time_t *__restrict, struct tm *__restrict);
int localtime_l(const struct timespec *__restrict, struct tm *__restrict,
                locale_t);
struct tm *localtime_r(const time_t *__restrict, struct tm *__restrict);
time_t mktime(struct tm *);
int mktime_l(const struct tm *__restrict, struct timespec *__restrict,
             locale_t);
int nanosleep(const struct timespec *, ...);
size_t strftime(char *__restrict, size_t, const char *__restrict,
                const struct tm *__restrict);
size_t strftime_l(char *__restrict, size_t, const char *__restrict,
                  const struct tm *__restrict, locale_t);
char *strptime(const char *__restrict, const char *__restrict,
               struct tm *__restrict);
char *strptime_l(const char *__restrict, const char *__restrict,
                 struct tm *__restrict, locale_t);
time_t time(time_t *);
time_t timegm(struct tm *);
int timespec_get(struct timespec *, int);
void tzset(void);
__END_DECLS

#endif
