// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/limits.h>
#include <common/locale.h>
#include <common/time.h>

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "time_impl.h"

// Determines which daylight saving time rule should be applied to a
// given time.
static const struct lc_timezone_rule *determine_applicable_rule(
    const struct lc_timezone_rule *rules, size_t rules_count,
    const struct tm *std, int gmtoff) {
  // Start out by picking the first standard time rule as a fallback. It
  // may be the case that the code below obtains no matching rule, for
  // example if the timestamp is lower than any of the starting times of
  // the rules.
  //
  // This rule has no influence on the time computation, but at least
  // ensures that we have a proper timezone abbreviation.
  static const struct lc_timezone_rule nomatch = {};
  const struct lc_timezone_rule *match = &nomatch;
  for (size_t i = 0; i < rules_count; ++i) {
    if (rules[i].save == 0) {
      match = &rules[i];
      break;
    }
  }

  // Obtain the rule that applies to the start of the current year, but
  // also the set of rules that applies to the current.
  struct ruleset ruleset = {};
  const struct lc_timezone_rule *unused;
  ruleset_fill_with_year(&ruleset, &rules, &rules_count, std->tm_year, &match,
                         &unused);

  // Determine whether there is a rule for the current year that matches
  // the time more accurately than the one matching the start of the
  // year.
  for (size_t i = 0; i < ruleset.rules_count; ++i) {
    const struct lc_timezone_rule *rule = ruleset.rules[i];
    int offset = 0;
    switch (rule->timebase) {
      case TIMEBASE_CUR:
        offset = match->save * 600;
        break;
      case TIMEBASE_UTC:
        offset = -gmtoff;
        break;
    }
    if (rule_is_greater_than(rule, std, offset))
      break;
    match = rule;
  }
  return match;
}

// Cached timezone abbreviations.
//
// struct tm::tm_zone needs to refer to timezone abbreviations by
// reference. This is problematic in cases where a timezone era uses
// "CE%sT" as its abbreviation and depends on daylight saving time rules
// to insert either "" or "S".
//
// Store cached timezone abbreviations in a very simple stringpool, so
// that every string is only allocated in memory once.
struct cached_abbreviation {
  struct cached_abbreviation *next;
  char abbreviation[16];
};

// Expands a timezone abbreviation containing "%s" and returns a copy of
// the string stored in the stringpool.
static const char *compute_combined_zone_abbreviation(const char *era,
                                                      const char *rule) {
  // Perform expansion.
  char abbreviation[sizeof(((struct cached_abbreviation *)0)->abbreviation)];
  snprintf(abbreviation, sizeof(abbreviation), era, rule);

  // See if there's already a copy of this string in the stringpool.
  static _Atomic(struct cached_abbreviation *) table = ATOMIC_VAR_INIT(NULL);
  struct cached_abbreviation *first =
      atomic_load_explicit(&table, memory_order_acquire);
  for (struct cached_abbreviation *ca = first; ca != NULL; ca = ca->next)
    if (strcmp(abbreviation, ca->abbreviation) == 0)
      return ca->abbreviation;

  // Allocate a copy to be stored in the stringpool.
  struct cached_abbreviation *ca_new = malloc(sizeof(*ca_new));
  if (ca_new == NULL)
    return "";
  strlcpy(ca_new->abbreviation, abbreviation, sizeof(ca_new->abbreviation));

  for (;;) {
    // Store the new entry in the global list. The list is lockless, so
    // perform a compare-and-exchange.
    ca_new->next = first;
    if (atomic_compare_exchange_weak(&table, &first, ca_new))
      return ca_new->abbreviation;

    // Compare-and-exchange failed. See if an entry for the same
    // abbreviation got created in the meantime.
    for (struct cached_abbreviation *ca = first; ca != ca_new->next;
         ca = ca->next)
      if (strcmp(abbreviation, ca->abbreviation) == 0) {
        free(ca_new);
        return ca->abbreviation;
      }
  }
}

// Computes the abbreviated name of the timezone. If possible, it
// returns a constant string. If not, it returns a copy of the string
// stored in the stringpool.
static const char *compute_zone_abbreviation(
    const struct lc_timezone_era *era, const struct lc_timezone_rule *rule) {
  // If the timezone already provides explicit abbreviations for
  // standard time and daylight saving time, use those directly.
  if (*era->abbreviation_dst != '\0')
    return rule->save == 0 ? era->abbreviation_std : era->abbreviation_dst;

  // If the era abbreviation contains %s, expand it with the
  // abbreviation of the rule. Keep track of the result in a string
  // pool, so we don't leak memory.
  if (strchr(era->abbreviation_std, '%') != NULL)
    return compute_combined_zone_abbreviation(era->abbreviation_std,
                                              rule->abbreviation);

  // Era abbreviation does not contain %s. Use the abbreviation of the
  // era or the rule, preferring the latter if three or more characters.
  return rule->abbreviation[0] != '\0' && rule->abbreviation[1] != '\0' &&
                 rule->abbreviation[2] != '\0'
             ? rule->abbreviation
             : era->abbreviation_std;
}

int localtime_l(const struct timespec *restrict timer,
                struct tm *restrict result, locale_t locale) {
  // Require tv_nsec to be in bounds, like other functions that accept
  // struct timespec.
  if (timer->tv_nsec < 0 || timer->tv_nsec >= NSEC_PER_SEC)
    return EINVAL;

  // Obtain the last era from the timezone that does not end before the
  // provided timestamp.
  const struct lc_timezone *tz = locale->timezone;
  const struct lc_timezone_era *era = &tz->eras[0];
  time_t era_start = NUMERIC_MIN(time_t);
  for (size_t i = 1; i < tz->eras_count; ++i) {
    if (era->end > timer->tv_sec)
      break;
    era_start = era->end + era->gmtoff + era->end_save * 600;
    era = &tz->eras[i];
  }

  int error;
  if (era->rules_count > 0) {
    // Timezone has daylight saving time rules. First compute the
    // standard time and use that to compute the actual offset. If the
    // timestamp is close to the start of the era and the UTC offset got
    // decreased, make sure that the timestamp used for matching is set
    // to the start of the era. This ensures that we match the proper
    // DST rules.
    time_t timer_std = timer->tv_sec + era->gmtoff;
    struct tm std;
    __localtime_utc(timer_std > era_start ? timer_std : era_start, &std);

    // Obtain applicable daylight saving time rule and recompute.
    const struct lc_timezone_rule *rule = determine_applicable_rule(
        era->rules, era->rules_count, &std, era->gmtoff);
    int gmtoff = era->gmtoff + rule->save * 600;
    error = __localtime_utc(timer->tv_sec + gmtoff, result);

    result->tm_isdst = rule->save > 0;
    result->tm_gmtoff = gmtoff;
    result->tm_zone = compute_zone_abbreviation(era, rule);
  } else {
    // Timezone has no daylight saving time rules. Compute local time
    // with timezone offset directly.
    error = __localtime_utc(timer->tv_sec + era->gmtoff, result);

    result->tm_gmtoff = era->gmtoff;
    static const struct lc_timezone_rule rule = {};
    result->tm_zone = compute_zone_abbreviation(era, &rule);
  }

  result->tm_nsec = timer->tv_nsec;
  return error;
}
