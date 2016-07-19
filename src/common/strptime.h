// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>

#include <limits.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

// Parses a number within a range from the input.
static bool parse_number_range(const char_t *restrict *buf, int low, int high,
                               int *result) {
  // We should see at least one digit.
  if (**buf < L'0' || **buf > L'9')
    return false;

  // Convert up to log10(high) digits.
  int value = 0;
  for (int max = high; max > 0 && **buf >= L'0' && **buf <= L'9'; max /= 10) {
    int newvalue = value * 10 + **buf - L'0';
    if (newvalue > high)
      break;
    value = newvalue;
    ++*buf;
  }

  // Value not within bounds.
  if (value < low)
    return false;
  *result = value;
  return true;
}

static bool parse_year(const char_t *restrict *buf, size_t field_width,
                       int *result, bool *negative) {
  // Parse leading plus or minus symbol.
  *negative = **buf == '-';
  if (**buf == '+' || **buf == '-') {
    ++*buf;
    --field_width;
  }

  int value = 0;
  if (field_width > 0) {
    // We should see at least one digit.
    if (**buf < L'0' || **buf > L'9')
      return false;

    // Convert up to a fixed number of digits.
    while (field_width-- > 0 && **buf >= L'0' && **buf <= L'9') {
      value = value * 10 + **buf - L'0';
      ++*buf;
    }
  }
  *result = *negative ? -value : value;
  return true;
}

char_t *NAME(const char_t *restrict buf, const char_t *restrict format,
             struct tm *restrict tm, locale_t locale) {
  struct tm result = {};
  int week;
  enum { NONE, SUNDAY, MONDAY, ISO_8601 } week_mode = NONE;
  int year_century = 19;
  int year_last2 = -1;
  bool year_negative = false;

  const struct lc_time *lc_time = locale->time;
  const wchar_t *subformat = L"";
  while (*format != '\0' || *subformat != L'\0') {
#define FORMAT_PEEK (*subformat != L'\0' ? *subformat : (wchar_t)*format)
#define FORMAT_NEXT (*subformat != L'\0' ? *subformat++ : (wchar_t)*format++)
    if (FORMAT_PEEK == L'%') {
      FORMAT_NEXT;

      // Skip flags.
      while (FORMAT_PEEK == L'0' || FORMAT_PEEK == L'+')
        FORMAT_NEXT;

      // Minimum field width.
      size_t field_width = 0;
      if (FORMAT_PEEK >= L'1' && FORMAT_PEEK <= L'9') {
        do {
          field_width = field_width * 10 + FORMAT_NEXT - '0';
        } while (FORMAT_PEEK >= L'0' && FORMAT_PEEK <= L'9');
      }

      // "E" and "O" modifiers.
      if (FORMAT_PEEK == L'E' || FORMAT_PEEK == L'O')
        FORMAT_NEXT;

      // Conversion specifiers.
      switch (FORMAT_NEXT) {
        case L'a':
        case L'A': {
          // Abbreviated or full weekday name.
          if (!parse_string(&buf, lc_time->day, 7, &result.tm_wday, locale) &&
              !parse_string(&buf, lc_time->abday, 7, &result.tm_wday, locale))
            return NULL;
          break;
        }
        case L'b':
        case L'B':
        case L'h': {
          // Abbreviated or full month name.
          if (!parse_string(&buf, lc_time->mon, 12, &result.tm_mon, locale) &&
              !parse_string(&buf, lc_time->abmon, 12, &result.tm_mon, locale))
            return NULL;
          break;
        }
        case L'c': {
          // Appropriate date and time representation.
          subformat = lc_time->d_t_fmt;
          break;
        }
        case L'C': {
          // Century.
          if (field_width == 0)
            field_width = 2;
          if (!parse_year(&buf, field_width, &year_century, &year_negative))
            return NULL;
          break;
        }
        case L'd':
        case L'e': {
          // Day of the month.
          if (!parse_number_range(&buf, 1, 31, &result.tm_mday))
            return NULL;
          break;
        }
        case L'D': {
          // Equivalent to "%m/%d/%y".
          subformat = L"%m/%d/%y";
          break;
        }
        case L'f': {
          // Extension: nanoseconds.
          if (*buf < L'0' || *buf > L'9')
            return NULL;
          result.tm_nsec = 0;
          for (long digit = 100000000; digit != 0; digit /= 10) {
            result.tm_nsec += (*buf++ - L'0') * digit;
            if (*buf < L'0' || *buf > L'9')
              break;
          }
          break;
        }
        case L'F': {
          // Equivalent to "%Y-%m-%d" with the field width applying to
          // the entire date string.
          field_width = field_width >= 6 ? field_width - 6 : 0;
          subformat = L"-%m-%d";
          goto year_number;
        }
        case L'g':
        case L'y': {
          // Last two digits of the year number. Whether this value
          // needs to be added or subtracted to tm_year depends on
          // whether the century is negative. Only process this data
          // after we've finished processing input.
          if (!parse_number_range(&buf, 0, 99, &year_last2))
            return NULL;
          break;
        }
        year_number:
        case L'G':
        case L'Y': {
          // Year number.
          bool negative;
          if (field_width == 0)
            field_width = 4;
          if (!parse_year(&buf, field_width, &result.tm_year, &negative))
            return NULL;
          result.tm_year -= 1900;
          break;
        }
        case L'H': {
          // 24-hour based hour.
          if (!parse_number_range(&buf, 0, 23, &result.tm_hour))
            return NULL;
          break;
        }
        case L'I': {
          // 12-hour based hour.
          int hour_12;
          if (!parse_number_range(&buf, 1, 12, &hour_12))
            return NULL;
          result.tm_hour = result.tm_hour / 12 * 12 + hour_12 % 12;
          break;
        }
        case L'j': {
          // Day of the year.
          if (!parse_number_range(&buf, 1, 366, &result.tm_yday))
            return NULL;
          --result.tm_yday;
          break;
        }
        case L'm': {
          // Month number.
          if (!parse_number_range(&buf, 1, 12, &result.tm_mon))
            return NULL;
          --result.tm_mon;
          break;
        }
        case L'M': {
          // Minutes.
          if (!parse_number_range(&buf, 0, 59, &result.tm_min))
            return NULL;
          break;
        }
        case L'n':
        case L't': {
          // Any white space.
          parse_whitespace(&buf, locale);
          break;
        }
        case L'p': {
          // "a.m." / "p.m." string.
          const wchar_t *ampm[2] = {lc_time->am_str, lc_time->pm_str};
          int pm;
          if (!parse_string(&buf, ampm, 2, &pm, locale))
            return NULL;
          result.tm_hour = pm ? result.tm_hour % 12 + 12 : result.tm_hour % 12;
          break;
        }
        case L'r': {
          // Time in a.m. and p.m. notation.
          subformat = lc_time->t_fmt_ampm;
          break;
        }
        case L'R': {
          // Time in 24-hour notation.
          subformat = L"%H:%M";
          break;
        }
        case L'S': {
          // Seconds.
          if (!parse_number_range(&buf, 0, 60, &result.tm_sec))
            return NULL;
          break;
        }
        case L'T': {
          // Equivalent to "%H:%M:%S".
          subformat = L"%H:%M:%S";
          break;
        }
        case L'u': {
          // Number of the weekday (Monday is 1, Sunday is 7).
          if (!parse_number_range(&buf, 1, 7, &result.tm_wday))
            return NULL;
          result.tm_wday %= 7;
          break;
        }
        case L'U': {
          // Number of the week (first Sunday starts week 1).
          if (!parse_number_range(&buf, 0, 53, &week))
            return NULL;
          week_mode = SUNDAY;
          break;
        }
        case L'V': {
          // Number of the week (ISO 8601).
          if (!parse_number_range(&buf, 1, 53, &week))
            return NULL;
          week_mode = ISO_8601;
          break;
        }
        case L'w': {
          // Number of the weekday (Sunday is 0, Saturday is 6).
          if (!parse_number_range(&buf, 0, 6, &result.tm_wday))
            return NULL;
          break;
        }
        case L'W': {
          // Number of the week (first Monday starts week 1).
          if (!parse_number_range(&buf, 0, 53, &week))
            return NULL;
          week_mode = MONDAY;
          break;
        }
        case L'x': {
          // Appropriate date representation.
          subformat = lc_time->d_fmt;
          break;
        }
        case L'X': {
          // Appropriate time representation.
          subformat = lc_time->t_fmt;
          break;
        }
        case L'z': {
          if ((buf[0] == L'-' || buf[0] == L'+') && buf[1] >= L'0' &&
              buf[1] <= L'9' && buf[2] >= L'0' && buf[2] <= L'9') {
            // Timezone offset in the form ±hh.
            result.tm_gmtoff =
                buf[1] * 36000 + buf[2] * 3600 - L'0' * 36000 - L'0' * 3600;
            int skip = 3;
            if (buf[3] >= L'0' && buf[3] <= L'5' && buf[4] >= L'0' &&
                buf[4] <= L'9') {
              // Timezone offset in the form ±hhmm.
              result.tm_gmtoff +=
                  buf[3] * 600 + buf[4] * 60 - L'0' * 600 - L'0' * 60;
              skip = 5;
            }
            if (buf[0] == L'-')
              result.tm_gmtoff = -result.tm_gmtoff;
            buf += skip;
          } else if (buf[0] == 'Z') {
            // Interpret "Z" as +0000 to ease parsing of ISO 8601.
            result.tm_gmtoff = 0;
            ++buf;
          }
          break;
        }
        case L'Z': {
          // Timezone name or abbreviation. We cannot store this
          // information in any way, so just skip it.
          while (*buf >= L'A' && *buf <= 'Z')
            ++buf;
          break;
        }
        case L'%': {
          // Ampersand.
          if (*buf++ != L'%')
            return NULL;
          break;
        }
      }
#undef FORMAT_PEEK
#undef FORMAT_NEXT
    } else if (*subformat != L'\0') {
      if (iswspace(*subformat)) {
        // Skip any whitespace.
        parse_whitespace(&buf, locale);
      } else {
#if WIDE
        // Match literal character.
        if (*buf++ != *subformat)
          return NULL;
#else
        // Match literal character after converting to multibyte.
        const struct lc_ctype *ctype = locale->ctype;
        char mb[MB_LEN_MAX];
        ssize_t len = ctype->c32tomb(mb, *subformat, ctype->data);
        if (len < 0)
          return NULL;
        for (size_t i = 0; i < (size_t)len; ++i)
          if (*buf++ != mb[i])
            return NULL;
#endif
      }
      ++subformat;
    } else {
      if (parse_whitespace(&format, locale)) {
        parse_whitespace(&buf, locale);
      } else if (*buf++ != *format++) {
        // Literal character did not match.
        return NULL;
      }
    }
  }

  // Compute the full year if a two-digit year is provided.
  if (year_last2 >= 0)
    result.tm_year = year_negative ? (year_century * 100 - year_last2) - 1900
                                   : (year_century * 100 + year_last2) - 1900;

  // Compute the date if a week number is provided.
  // TODO(ed): Implement.

  *tm = result;
  return (char_t *)buf;
}
