// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <limits.h>
#include <strings.h>

#include "ctype/asciiplus.h"
#include "ctype/gb2312.h"
#include "ctype/ibm037.h"
#include "ctype/iso_8859_1.h"
#include "ctype/iso_8859_15.h"

const struct lc_ctype *__lookup_ctype(const char *name, size_t len) {
  static const struct {
    const char name[19];
    const struct lc_ctype *ctype;
  } ctypes[] = {
      // IANA names and aliases for character sets.
      {"GB2312\0", &ctype_gb2312},
      {"csGB2312\0", &ctype_gb2312},

      {"IBM037\0", &ctype_ibm037},
      {"cp037\0", &ctype_ibm037},
      {"ebcdic-cp-us\0", &ctype_ibm037},
      {"ebcdic-cp-ca\0", &ctype_ibm037},
      {"ebcdic-cp-wt\0", &ctype_ibm037},
      {"ebcdic-cp-nl\0", &ctype_ibm037},
      {"csIBM037\0", &ctype_ibm037},

      {"ISO-8859-1\0", &ctype_iso_8859_1},
      {"ISO_8859-1:1987\0", &ctype_iso_8859_1},
      {"iso-ir-100\0", &ctype_iso_8859_1},
      {"ISO_8859-1\0", &ctype_iso_8859_1},
      {"latin1\0", &ctype_iso_8859_1},
      {"l1\0", &ctype_iso_8859_1},
      {"IBM819\0", &ctype_iso_8859_1},
      {"CP819\0", &ctype_iso_8859_1},
      {"csISOLatin1\0", &ctype_iso_8859_1},

      {"ISO-8859-2\0", &ctype_iso_8859_2},
      {"ISO_8859-2:1987\0", &ctype_iso_8859_2},
      {"iso-ir-101\0", &ctype_iso_8859_2},
      {"ISO_8859-2\0", &ctype_iso_8859_2},
      {"latin2\0", &ctype_iso_8859_2},
      {"l2\0", &ctype_iso_8859_2},
      {"csISOLatin2\0", &ctype_iso_8859_2},

      {"ISO-8859-5\0", &ctype_iso_8859_5},
      {"ISO_8859-5:1988\0", &ctype_iso_8859_5},
      {"iso-ir-144\0", &ctype_iso_8859_5},
      {"ISO_8859-5\0", &ctype_iso_8859_5},
      {"cyrillic\0", &ctype_iso_8859_5},
      {"csISOLatinCyrillic\0", &ctype_iso_8859_5},

      {"ISO-8859-9\0", &ctype_iso_8859_9},
      {"iso-ir-148\0", &ctype_iso_8859_9},
      {"ISO_8859-9\0", &ctype_iso_8859_9},
      {"latin5\0", &ctype_iso_8859_9},
      {"l5\0", &ctype_iso_8859_9},
      {"csISOLatin5\0", &ctype_iso_8859_9},

      {"ISO-8859-13\0", &ctype_iso_8859_13},
      {"csISO885913\0", &ctype_iso_8859_13},

      {"ISO-8859-15\0", &ctype_iso_8859_15},
      {"ISO_8859-15\0", &ctype_iso_8859_15},
      {"Latin-9\0", &ctype_iso_8859_15},
      {"csISO885915\0", &ctype_iso_8859_15},

      {"KOI8-R\0", &ctype_koi8_r},
      {"csKOI8R\0", &ctype_koi8_r},

      {"KOI8-U\0", &ctype_koi8_u},
      {"csKOI8U\0", &ctype_koi8_u},

      {"US-ASCII\0", &__ctype_us_ascii},
      {"iso-ir-6\0", &__ctype_us_ascii},
      {"ANSI_X3.4-1968\0", &__ctype_us_ascii},
      {"ANSI_X3.4-1986\0", &__ctype_us_ascii},
      {"ISO_646.irv:1991\0", &__ctype_us_ascii},
      {"ISO646-US\0", &__ctype_us_ascii},
      {"us\0", &__ctype_us_ascii},
      {"IBM367\0", &__ctype_us_ascii},
      {"cp367\0", &__ctype_us_ascii},
      {"csASCII\0", &__ctype_us_ascii},

      {"UTF-8\0", &__ctype_utf_8},
      {"csUTF8\0", &__ctype_utf_8},

      {"Windows-874\0", &ctype_windows_874},
      {"cswindows874\0", &ctype_windows_874},

      {"Windows-1250\0", &ctype_windows_1250},
      {"cswindows1250\0", &ctype_windows_1250},

      {"Windows-1251\0", &ctype_windows_1251},
      {"cswindows1251\0", &ctype_windows_1251},

      {"Windows-1252\0", &ctype_windows_1252},
      {"cswindows1252\0", &ctype_windows_1252},

      {"Windows-1253\0", &ctype_windows_1253},
      {"cswindows1253\0", &ctype_windows_1253},

      {"Windows-1254\0", &ctype_windows_1254},
      {"cswindows1254\0", &ctype_windows_1254},

      {"Windows-1255\0", &ctype_windows_1255},
      {"cswindows1255\0", &ctype_windows_1255},

      {"Windows-1256\0", &ctype_windows_1256},
      {"cswindows1256\0", &ctype_windows_1256},

      {"Windows-1257\0", &ctype_windows_1257},
      {"cswindows1257\0", &ctype_windows_1257},

      {"Windows-1258\0", &ctype_windows_1258},
      {"cswindows1258\0", &ctype_windows_1258},

      // Names for compatibility with other implementations.
      {"EUC-CN\0", &ctype_gb2312},
      {"ISO8859-1\0", &ctype_iso_8859_1},
      {"ISO8859-2\0", &ctype_iso_8859_2},
      {"ISO8859-5\0", &ctype_iso_8859_5},
      {"ISO8859-9\0", &ctype_iso_8859_9},
      {"ISO8859-13\0", &ctype_iso_8859_13},
      {"ISO8859-15\0", &ctype_iso_8859_15},
  };
  static_assert(sizeof(ctypes[0].name) <= CHARCLASS_NAME_MAX,
                "CHARCLASS_NAME_MAX too low");

  // Look up first entry with matching name.
  for (size_t i = 0; i < __arraycount(ctypes); ++i) {
    if (strncasecmp(name, ctypes[i].name, len) == 0 &&
        ctypes[i].name[len] == '\0')
      return ctypes[i].ctype;
  }
  return NULL;
}
