// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>
#include <common/time.h>

#include <string.h>
#include <time.h>

#include "time_impl.h"

// Determines which daylight saving time rule should be applied to a
// given time.
static const struct lc_timezone_rule *determine_applicable_rule(
    const struct lc_timezone_rule *rules, size_t rules_count,
    const struct tm *std, int gmtoff) {
  // Obtain the rule that applies to the start of the current year, but
  // also the set of rules that applies to the current.
  struct ruleset ruleset = {};
  const struct lc_timezone_rule *match = NULL, *unused;
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
        offset = match != NULL ? match->save * 600 : 0;
        break;
      case TIMEBASE_UTC:
        offset = -gmtoff;
        break;
    }
    if (rule_is_greater_than(rule, std, offset))
      break;
    match = rule;
  }

  if (match != NULL)
    return match;

  // Still not a single rule that matched. Find at least an entry
  // without daylight saving time, so we have a proper timezone name.
  while (rules_count > 0) {
    if (rules->save == 0)
      return rules;
    ++rules;
    --rules_count;
  }

  // Not a single rule that is of any use.
  static const struct lc_timezone_rule nomatch = {};
  return &nomatch;
}

// Computes the abbreviated name of the timezone. For example, the era
// may use the template "CE%sT". Daylight saving time rules may then use
// the abbreviation "" or "S" to yield "CET" or "CEST", respectively.
static void compute_zone_abbreviation(const struct lc_timezone_era *era,
                                      const struct lc_timezone_rule *rule,
                                      char *result, size_t resultsize) {
  if (*era->abbreviation_dst != '\0') {
    // Timezone already provides explicit abbreviations for standard
    // time and daylight saving time. Copy those to the output.
    if (rule->save == 0) {
      if (resultsize > sizeof(era->abbreviation_std))
        resultsize = sizeof(era->abbreviation_std) + 1;
      strlcpy(result, era->abbreviation_std, resultsize);
    } else {
      if (resultsize > sizeof(era->abbreviation_dst))
        resultsize = sizeof(era->abbreviation_dst) + 1;
      strlcpy(result, era->abbreviation_dst, resultsize);
    }
  } else {
    for (size_t i = 0; i < sizeof(era->abbreviation_std) - 1; ++i) {
      if (era->abbreviation_std[i] == '%' &&
          era->abbreviation_std[i + 1] == 's') {
        // Era abbreviation contains %s. In this case, expand the %s
        // with the abbreviation of the rule (e.g., "CE%sT" -> "CEST").
        size_t in = 0, out = 0;
        while (in < sizeof(era->abbreviation_std) && out + 1 < resultsize) {
          if (era->abbreviation_std[in] == '%' &&
              era->abbreviation_std[in + 1] == 's') {
            // %s directive.
            for (size_t j = 0;
                 j < sizeof(rule->abbreviation) &&
                     rule->abbreviation[j] != '\0' && out + 1 < resultsize;
                 ++j)
              result[out++] = rule->abbreviation[j];
            in += 2;
          } else {
            // Regular character.
            result[out++] = era->abbreviation_std[in++];
          }
        }
        result[out] = '\0';
        return;
      }
    }

    // Era abbreviation does not contain %s. Use the abbreviation of the
    // era or the rule, preferring the latter if non-empty.
    if (*rule->abbreviation == '\0') {
      if (resultsize > sizeof(era->abbreviation_std))
        resultsize = sizeof(era->abbreviation_std) + 1;
      strlcpy(result, era->abbreviation_std, resultsize);
    } else {
      if (resultsize > sizeof(rule->abbreviation))
        resultsize = sizeof(rule->abbreviation) + 1;
      strlcpy(result, rule->abbreviation, resultsize);
    }
  }
}

int localtime_l(const struct timespec *restrict timer,
                struct tm *restrict result, locale_t locale) {
  // Require tv_nsec to be in bounds, like other functions that accept
  // struct timespec.
  if (timer->tv_nsec < 0 || timer->tv_nsec >= NSEC_PER_SEC)
    return EINVAL;

  // Obtain the last era from the timezone that does not end before the
  // provided timestamp.
  const struct lc_timezone *timezone = locale->timezone;
  const struct lc_timezone_era *era = &timezone->eras[0];
  for (size_t i = 1; i < timezone->eras_count; ++i) {
    if (era->end > timer->tv_sec)
      break;
    era = &timezone->eras[i];
  }

  if (era->rules_count > 0) {
    // Timezone has daylight saving time rules. First compute the
    // standard time and use that to compute the actual offset.
    struct tm std;
    int error = __localtime_utc(timer->tv_sec + era->gmtoff, &std);
    if (error != 0)
      return error;

    // Obtain applicable daylight saving time rule and recompute.
    const struct lc_timezone_rule *rule = determine_applicable_rule(
        era->rules, era->rules_count, &std, era->gmtoff);
    int gmtoff = era->gmtoff + rule->save * 600;
    error = __localtime_utc(timer->tv_sec + gmtoff, result);
    if (error != 0)
      return error;

    result->tm_isdst = rule->save > 0;
    result->tm_gmtoff = gmtoff;
    compute_zone_abbreviation(era, rule, result->tm_zone,
                              sizeof(result->tm_zone));
  } else {
    // Timezone has no daylight saving time rules. Compute local time
    // with timezone offset directly.
    int error = __localtime_utc(timer->tv_sec + era->gmtoff, result);
    if (error != 0)
      return error;

    result->tm_gmtoff = era->gmtoff;
    static const struct lc_timezone_rule rule = {};
    compute_zone_abbreviation(era, &rule, result->tm_zone,
                              sizeof(result->tm_zone));
  }

  result->tm_nsec = timer->tv_nsec;
  return 0;
}
