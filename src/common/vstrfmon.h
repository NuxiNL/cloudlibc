// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/types.h>

#include <stdarg.h>

#if WIDE
#include <wchar.h>
typedef wchar_t char_t;
#else
#include <monetary.h>
typedef char char_t;
#endif

ssize_t NAME(char_t *restrict s, size_t maxsize, locale_t locale,
             const char_t *restrict format, va_list ap) {
  // TODO(edje): Implement.
  return -1;
}
