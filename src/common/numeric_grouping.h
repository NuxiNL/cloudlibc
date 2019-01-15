// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_NUMERIC_GROUPING_H
#define COMMON_NUMERIC_GROUPING_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Utility functions for inserting grouping characters into a number.
//
// The grouping string stored in LC_NUMERIC and LC_MONETARY has the
// disadvantage that it encodes the grouping of a number from right to
// left, whereas we would typically want to print numbers from left to
// right. The last entry in the grouping string may also be repeated.
//
// This interface converts the grouping string to a simple state machine
// that returns true or false depending on whether a grouping character
// should be inserted. Example code:
//
//   char digits[7] = "1234567";
//   struct numeric_grouping ng;
//   size_t n = numeric_grouping_init(&ng, "\x03", sizeof(digits));
//   char grouped_digits[sizeof(digits) + n];
//   char *out = grouped_digits;
//   for (size_t i = 0; i < sizeof(digits); ++i) {
//     if (numeric_grouping_step(&ng))
//       *out++ = ',';
//     *out++ = digits[i];
//   }
//
// The grouped_digits array will now contain "1,234,567".

struct numeric_grouping {
  const signed char *grouping;
  size_t steps;
  size_t repetitions;
};

static inline size_t numeric_grouping_init(struct numeric_grouping *ng,
                                           const signed char *grouping,
                                           size_t ndigits) {
  if (ndigits <= 1 || grouping == NULL || *grouping == '\0') {
    // Number is too short, the grouping string is empty. Don't attempt
    // to do any grouping.
    ng->steps = SIZE_MAX;
    return 0;
  }

  size_t ngroupings = 0;
  for (;;) {
    assert(ndigits >= 1 && "Attempted to format short number");
    assert(*grouping != '\0' && "Unexpected null byte");
    if (*grouping < '\0' || ndigits <= (size_t)*grouping) {
      // Don't do any grouping anymore. Place all of the remaining
      // digits in a group.
      *ng = (struct numeric_grouping){
          .grouping = grouping,
          .steps = ndigits,
      };
      return ngroupings;
    } else if (grouping[1] == '\0') {
      // Continue to do the grouping the same way indefinitely.
      *ng = (struct numeric_grouping){
          .grouping = grouping,
          .steps = (ndigits - 1) % *grouping + 1,
          .repetitions = (ndigits - 1) / *grouping,
      };
      return ngroupings + ng->repetitions;
    } else {
      // Still more groups to go after this one.
      ndigits -= *grouping;
      ++grouping;
      ++ngroupings;
    }
  }
}

static inline bool numeric_grouping_step(struct numeric_grouping *ng) {
  if (ng->steps != 0) {
    // Still more steps to go before we need to print a grouping character.
    --ng->steps;
    return false;
  }

  // Reload the number of steps we need.
  if (ng->repetitions != 0) {
    // Repeat the same group another time.
    --ng->repetitions;
  } else {
    // No more repetitions of this group. Move on to the next group.
    --ng->grouping;
  }
  assert(*ng->grouping > 0 && "Attempted to reload zero steps");
  ng->steps = *ng->grouping - 1;
  return true;
}

#endif
