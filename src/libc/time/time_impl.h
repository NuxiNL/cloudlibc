// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef TIME_TIME_IMPL_H
#define TIME_TIME_IMPL_H

#include <common/locale.h>
#include <common/time.h>

#include <sys/types.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// A set of daylight saving rules that are active for a certain year. We
// normally assume that for every year, we have up to two rules. One
// rule to enable daylight saving time and one rule to disable it (the
// ordering depending on the hemisphere).
//
// Still, there are certain countries that once experimented with double
// summer time, meaning there are four rules. Be conservative and allow
// up to 8 rules per year.
//
// Rules are stored in sorted order within the year.
struct ruleset {
  const struct lc_timezone_rule *rules[8];
  size_t rules_count;
};

// Removes rules that don't apply to this year anymore.
static inline void ruleset_remove_stale(struct ruleset *ruleset, int year) {
  size_t old_rules_count = ruleset->rules_count;
  ruleset->rules_count = 0;
  for (size_t i = 0; i < old_rules_count; ++i) {
    if (ruleset->rules[i]->year_to == UINT8_MAX ||
        ruleset->rules[i]->year_to >= year) {
      ruleset->rules[ruleset->rules_count++] = ruleset->rules[i];
    }
  }
}

// Inserts a new rule into the ruleset, while maintaining the order of
// the rules.
static inline void ruleset_add(struct ruleset *ruleset,
                               const struct lc_timezone_rule *rule) {
  assert(ruleset->rules_count < __arraycount(ruleset->rules) &&
         "Timezone ruleset has too many rules for the same year");
  size_t newslot = ruleset->rules_count++;
  while (newslot > 0 &&
         (rule->month < ruleset->rules[newslot - 1]->month ||
          (rule->month == ruleset->rules[newslot - 1]->month &&
           rule->monthday < ruleset->rules[newslot - 1]->monthday))) {
    assert((rule->month != ruleset->rules[newslot - 1]->month ||
            rule->monthday != ruleset->rules[newslot - 1]->monthday) &&
           "Timezone has two rules for the same day");
    ruleset->rules[newslot] = ruleset->rules[newslot - 1];
    --newslot;
  }
  ruleset->rules[newslot] = rule;
}

// Returns the last entry that got applied before the start of a year.
// Even though entries in the ruleset are sorted by month and day, it
// may not be the case that this is the last entry in the ruleset. Some
// entries may expire earlier than others.
//
// Pick the entry that has the highest month and day number, prefering a
// shorter expiration.
static inline void ruleset_get_last(struct ruleset *ruleset, int year,
                                    const struct lc_timezone_rule **last,
                                    const struct lc_timezone_rule **last_dst) {
  assert(ruleset->rules_count > 0 && "Cannot be called on an empty ruleset");
  *last = ruleset->rules[ruleset->rules_count - 1];
  if ((*last)->save > 0)
    *last_dst = *last;
  for (ssize_t i = ruleset->rules_count - 2;
       (*last)->year_to < year - 1 && i >= 0; --i) {
    const struct lc_timezone_rule *rule = ruleset->rules[i];
    if ((*last)->year_to < rule->year_to) {
      *last = rule;
      if ((*last)->save > 0)
        *last_dst = *last;
    }
  }
}

// Populates a ruleset with a set of rules that applies to a given year.
// During the process, it also keeps track of the rule that applies
// to the start of the year.
//
// In addition to that, it also stores the rule that applies to the
// start of the year, under the assumption that non-DST rules are
// ignored. This is used by mktime() in case tm_isdst is set to 1.
static inline void ruleset_fill_with_year(
    struct ruleset *ruleset, const struct lc_timezone_rule **rules,
    size_t *rules_count, int year, const struct lc_timezone_rule **last,
    const struct lc_timezone_rule **last_dst) {
  // Step 1: Determine the set of rules of the previous year, so we can
  // obtain the offset at the end of that year.
  int last_year = 0;
  while (*rules_count > 0 && (*rules)->year_from < year) {
    const struct lc_timezone_rule *rule = (*rules)++;
    if (last_year != rule->year_from)
      ruleset_remove_stale(ruleset, rule->year_from);
    ruleset_add(ruleset, rule);
    ruleset_get_last(ruleset, year, last, last_dst);
    --*rules_count;
  }

  // Step 2: Determine the set of rules that apply to the current year.
  ruleset_remove_stale(ruleset, year);
  while (*rules_count > 0 && (*rules)->year_from == year) {
    ruleset_add(ruleset, (*rules)++);
    --*rules_count;
  }
}

// Returns whether the start date of a daylight saving time rule is
// later than the time provided.
static inline bool rule_is_greater_than(const struct lc_timezone_rule *rule,
                                        const struct tm *tm, int offset) {
  // Apply the offset to the time and obtain the time of day in seconds.
  int seconds = tm->tm_sec + tm->tm_min * 60 + tm->tm_hour * 3600 + offset;
  int monthday = tm->tm_mday + seconds / 86400;
  int weekday = tm->tm_wday + seconds / 86400;
  seconds %= 86400;
  if (seconds < 0) {
    seconds += 86400;
    --weekday;
    --monthday;
  }

  // Normalize the month.
  int month = tm->tm_mon;
  if (monthday < 1) {
    // Time went back into last year.
    if (month == 0)
      return true;
    monthday += get_months(tm->tm_year)[month - 1];
    --month;
  } else {
    int monthlen = get_months(tm->tm_year)[month];
    if (monthday > monthlen) {
      monthday -= monthlen;
      ++month;
    }
  }

  // Compare months.
  if (rule->month != month)
    return rule->month > month;

  // When rule->weekday is between 0 and 6, we only want to apply the
  // rule on the first occurrence of the weekday on or after the day
  // of the month. More concretely, if weekday == 3 and monthday ==
  // 15, the rule only takes effect on the first Wednesday on or after
  // the 15th of that month.
  int monthdaydelta = 0;
  if (rule->weekday != 7) {
    monthdaydelta =
        ((int)rule->weekday - weekday + monthday - (int)rule->monthday) % 7;
    if (monthdaydelta < 0)
      monthdaydelta += 7;
  }

  // Compare day of the month and time.
  return (rule->monthday + monthdaydelta) * 86400 + rule->minute * 60 >
         monthday * 86400 + seconds;
}

#endif
