// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#if WIDE
#include <wchar.h>
typedef wchar_t char_t;
#else
#include <string.h>
typedef char char_t;
#endif

enum chartype {
  T_ZERO,   // Character is '0'.
  T_DIGIT,  // Character is between '1' and '9'.
  T_OTHER,  // Character is something else.
};

enum state {
  S_TEXT,   // Currently in non-numerical text.
  S_FRAC,   // Currently in a fractional version number.
  S_FRAC0,  // Currently in a fractional version number with value zero.
  S_INT,    // Currently in an integer version number.
};

enum retmode {
  R_LT = 1,      // Return -1.
  R_GT = 2,      // Return 1.
  R_LENGTH = 3,  // Return the difference, taking the length into consideration.
};

// Returns the type of a character.
static enum chartype get_chartype(char_t c) {
  if (c == '0')
    return T_ZERO;
  if (c >= '1' && c <= '9')
    return T_DIGIT;
  return T_OTHER;
}

int NAME(const char_t *s1, const char_t *s2) {
  // Walk over the input strings until we reach the first character
  // mismatch. Keep track of whether we're in an integer, a fraction or
  // just text.
  enum state state = S_TEXT;
  while (*s1 == *s2) {
    if (*s1 == '\0')
      return 0;
#define I(s, ct) (((s) + (ct)*4))
    static const char next[] = {
        [I(S_TEXT, T_ZERO)] = S_FRAC0,  [I(S_TEXT, T_DIGIT)] = S_INT,
        [I(S_TEXT, T_OTHER)] = S_TEXT,

        [I(S_FRAC, T_ZERO)] = S_FRAC,   [I(S_FRAC, T_DIGIT)] = S_FRAC,
        [I(S_FRAC, T_OTHER)] = S_TEXT,

        [I(S_FRAC0, T_ZERO)] = S_FRAC0, [I(S_FRAC0, T_DIGIT)] = S_FRAC,
        [I(S_FRAC0, T_OTHER)] = S_TEXT,

        [I(S_INT, T_ZERO)] = S_INT,     [I(S_INT, T_DIGIT)] = S_INT,
        [I(S_INT, T_OTHER)] = S_TEXT,
    };
    static_assert(sizeof(next) == 4 * 3, "Incorrect size");
    state = next[I(state, get_chartype(*s1))];
#undef I
    ++s1;
    ++s2;
  }

  // Determine whether the function should return the difference of the
  // two characters, a positive value, or a negative value.
  int diff = *s1 < *s2 ? -1 : 1;
#define I(s, ct1, ct2) ((s) + (ct1)*4 + (ct2)*12)
  static const char mode[4 * 3 * 3] = {
      [I(S_TEXT, T_DIGIT, T_DIGIT)] = R_LENGTH,

      [I(S_FRAC, T_ZERO, T_OTHER)] = R_GT,
      [I(S_FRAC, T_DIGIT, T_OTHER)] = R_GT,
      [I(S_FRAC, T_OTHER, T_ZERO)] = R_LT,
      [I(S_FRAC, T_OTHER, T_DIGIT)] = R_LT,

      [I(S_FRAC0, T_ZERO, T_OTHER)] = R_LT,
      [I(S_FRAC0, T_DIGIT, T_OTHER)] = R_LT,
      [I(S_FRAC0, T_OTHER, T_ZERO)] = R_GT,
      [I(S_FRAC0, T_OTHER, T_DIGIT)] = R_GT,

      [I(S_INT, T_ZERO, T_DIGIT)] = R_LENGTH,
      [I(S_INT, T_ZERO, T_OTHER)] = R_GT,
      [I(S_INT, T_DIGIT, T_ZERO)] = R_LENGTH,
      [I(S_INT, T_DIGIT, T_DIGIT)] = R_LENGTH,
      [I(S_INT, T_DIGIT, T_OTHER)] = R_GT,
      [I(S_INT, T_OTHER, T_ZERO)] = R_LT,
      [I(S_INT, T_OTHER, T_DIGIT)] = R_LT,
  };
  switch (mode[I(state, get_chartype(*s1++), get_chartype(*s2++))]) {
#undef I
    case R_LT:
      return -1;
    case R_GT:
      return 1;
    case R_LENGTH:
      for (;;) {
        enum chartype ct1 = get_chartype(*s1++);
        enum chartype ct2 = get_chartype(*s2++);
        if (ct1 == T_OTHER)
          return ct2 == T_OTHER ? diff : -1;
        else if (ct2 == T_OTHER)
          return 1;
      }
    default:
      return diff;
  }
}
