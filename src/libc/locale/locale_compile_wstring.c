// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

static char *convert_wstring(const wchar_t *in, locale_t locale) {
  // Compute length of multi-byte string.
  const struct lc_ctype *ctype = locale->ctype;
  const char32_t *wcs = (const char32_t *)in;
  ssize_t len = ctype->c32stombs(NULL, 0, &wcs, SIZE_MAX, ctype->data);
  if (len < 0)
    return NULL;

  // Allocate memory and convert the wide-character string.
  char *str = malloc(len + 1);
  if (str == NULL)
    return NULL;
  ctype->c32stombs(str, len, &wcs, SIZE_MAX, ctype->data);
  str[len] = '\0';
  return str;
}

const char *__locale_compile_wstring(locale_t locale,
                                     const wchar_t *uncompiled_string,
                                     size_t compiled_offset) {
  // No value available.
  if (uncompiled_string == NULL)
    return "";

  struct lc_compiled *compiled = __locale_get_compiled(locale);
  if (compiled == NULL)
    return "";

  // See if we've requested this string before.
  _Atomic(char *) *compiled_string_ref =
      (void *)((uintptr_t)compiled + compiled_offset);
  char *compiled_string = atomic_load(compiled_string_ref);
  if (compiled_string == NULL) {
    // String has not been requested before. Compile it now.
    char *new_compiled_string = convert_wstring(uncompiled_string, locale);
    if (new_compiled_string == NULL)
      return "";
    if (atomic_compare_exchange_strong(compiled_string_ref, &compiled_string,
                                       new_compiled_string)) {
      compiled_string = new_compiled_string;
    } else if (new_compiled_string) {
      // Race condition: another thread also computed a copy of this string.
      free(new_compiled_string);
    }
  }
  return compiled_string;
}
