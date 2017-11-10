// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <assert.h>
#include <limits.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Determines whether a multi-byte string has the same representation in
// two character sets.
static bool string_has_same_representation(const char *str,
                                           const struct lc_ctype *from,
                                           const struct lc_ctype *to) {
  // Prevent comparison if needed.
  if (from == to)
    return true;

  // Decode/encode characters one by one.
  mbstate_t mbs;
  mbstate_set_init(&mbs);
  while (*str != '\0') {
    char32_t ch;
    ssize_t len = from->mbtoc32(&ch, str, MB_LEN_MAX, &mbs, from->data);
    assert(len > 0 && "Failed to decode compile-time string");

    // Compare with output representation.
    char buf[MB_LEN_MAX];
    if (to->c32tomb(buf, ch, to->data) != len || memcmp(str, buf, len) != 0)
      return false;
    str += len;
  }

  // String is identical.
  return true;
}

const char *__locale_compile_string(locale_t locale,
                                    const char *uncompiled_string,
                                    size_t compiled_offset) {
  // Compile-time strings are always UTF-8.
  const struct lc_ctype *ctype_in = &__ctype_utf_8;
  const struct lc_ctype *ctype_out = locale->ctype;

  // We can also reuse the compile-string if its representation is
  // identical in both character sets.
  if (string_has_same_representation(uncompiled_string, ctype_in, ctype_out))
    return (char *)uncompiled_string;

#if 0
  struct lc_compiled *compiled = __locale_get_compiled(locale);
  if (compiled == NULL)
    return "";

  // See if we've requested this string before.
  _Atomic(char *)*compiled_string_ref =
      (void *)((uintptr_t)compiled + compiled_offset);
  char *compiled_string = atomic_load(compiled_string_ref);
  if (compiled_string == NULL) {
    // String has not been requested before. Compile it now.
    char *new_compiled_string = convert_string(uncompiled_string,
                                               ctype_in, ctype_out);
    if (new_compiled_string == NULL)
      return "";
    if (atomic_compare_exchange_strong(compiled_string_ref, &compiled_string,
                                       new_compiled_string)) {
      compiled_string = new_compiled_string;
    } else {
      // Race condition: another thread also computed a copy of this string.
      free(new_compiled_string);
    }
  }
  return compiled_string;
#else
  // TODO(ed): Implement.
  return (char *)"";
#endif
}
