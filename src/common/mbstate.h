// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_MBSTATE_H
#define COMMON_MBSTATE_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <uchar.h>

// Functions for interacting with mbstate_t objects.
//
// In this implementation, mbstate_t objects are used for multiple
// purposes. First of all, they are used by c16rtomb() and mbrtoc16() to
// keep track track of surrogate halves. Second, they are used by the
// UTF-8 and GB 2312 parsers to keep track of partial characters.
//
// These functions ensure that mbstate_t objects are either in the
// initial state, hold a surrogate half, or store a partially parsed
// character. It also allows the address of the mbstate_t to be NULL,
// provided that it never leaves the initial state. This allows us to
// keep calls to wide character functions working for single-byte
// character sets (ASCII, ISO-8859-1).

// Returns true when the mbstate_t is in its initial state.
static inline bool mbstate_get_init(const mbstate_t *mbs) {
  return mbs == NULL ||
         ((mbs->__surrogate < 0xd800 || mbs->__surrogate > 0xdfff) &&
          mbs->__bytesleft == 0);
}

// Places the mbstate_t in its initial state.
static inline void mbstate_set_init(mbstate_t *mbs) {
  if (mbs != NULL)
    *mbs = (mbstate_t){};
}

// Returns true when the mbstate_t contains a leading or trailing
// surrogate, providing access to its value.
static inline bool mbstate_get_surrogate(const mbstate_t *mbs,
                                         char16_t *surrogate) {
  if (mbs->__surrogate < 0xd800 || mbs->__surrogate > 0xdfff)
    return false;
  *surrogate = mbs->__surrogate;
  return true;
}

// Stores a leading or trailing surrogate in the mbstate_t.
static inline void mbstate_set_surrogate(mbstate_t *mbs, char16_t surrogate) {
  assert(surrogate >= 0xd800 && surrogate <= 0xdfff && "Bad surrogate");
  *mbs = (mbstate_t){.__surrogate = surrogate};
}

// Gets the multibyte character parsing state from the mbstate_t.
static inline void mbstate_get_multibyte(const mbstate_t *mbs,
                                         unsigned int *bytesleft,
                                         char32_t *partial,
                                         char32_t *lowerbound) {
  *bytesleft = mbs->__bytesleft;
  *partial = mbs->__partial;
  *lowerbound = mbs->__lowerbound;
}

// Sets the multibyte character parsing state in the mbstate_t.
static inline void mbstate_set_multibyte(mbstate_t *mbs, unsigned int bytesleft,
                                         char32_t partial,
                                         char32_t lowerbound) {
  *mbs = (mbstate_t){
      .__bytesleft = bytesleft,
      .__partial = partial,
      .__lowerbound = lowerbound,
  };
}

#endif
