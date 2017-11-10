// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/time.h>

#include <stdint.h>
#include <time.h>
#include <wchar.h>

#if WIDE
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

static int iso_8601_yday(int yday, int wday) {
  // Compute the first Thursday of the year.
  int firstthursday = (yday - wday + 4) % 7;
  if (firstthursday < 0)
    firstthursday += 7;
  // Compute day of year relative to the Monday of that week.
  return yday - firstthursday + 3;
}

static int iso_8601_week(const struct tm *tm) {
  int yday = iso_8601_yday(tm->tm_yday, tm->tm_wday);
  if (yday < 0) {
    // Day belongs to the previous year.
    yday = iso_8601_yday(tm->tm_yday + get_ydays(tm->tm_year - 1), tm->tm_wday);
  } else {
    // Day might belong to the next year.
    int nextyday =
        iso_8601_yday(tm->tm_yday - get_ydays(tm->tm_year), tm->tm_wday);
    if (nextyday >= 0)
      yday = nextyday;
  }
  return yday / 7 + 1;
}

static inline int iso_8601_year(const struct tm *tm) {
  // Check if day belongs to the previous year.
  if (iso_8601_yday(tm->tm_yday, tm->tm_wday) < 0)
    return tm->tm_year - 1;
  // Check if day belongs to the next year.
  if (iso_8601_yday(tm->tm_yday - get_ydays(tm->tm_year), tm->tm_wday) >= 0)
    return tm->tm_year + 1;
  // Day belongs to the current year.
  return tm->tm_year;
}

size_t NAME(char_t *restrict s, size_t maxsize, const char_t *restrict format,
            const struct tm *restrict timeptr, locale_t locale) {
  // Not even space for a nul byte. Return immediately.
  if (maxsize == 0)
    return 0;

  const struct lc_time *lc_time = locale->time;
  const wchar_t *subformat = L"";
  size_t nstored = 0;
#define PUTCHAR(c)              \
  do {                          \
    if (nstored + 1 >= maxsize) \
      return 0;                 \
    s[nstored++] = (c);         \
  } while (0)
  while (*format != '\0' || *subformat != L'\0') {
#define FORMAT_PEEK (*subformat != L'\0' ? *subformat : (wchar_t)*format)
#define FORMAT_NEXT (*subformat != L'\0' ? *subformat++ : (wchar_t)*format++)
    if (FORMAT_PEEK == L'%') {
      FORMAT_NEXT;

      // Parse flags.
      char left_padding = ' ';
      bool plus = false;
      for (;;) {
        if (FORMAT_PEEK == L'0') {
          left_padding = '0';
        } else if (FORMAT_PEEK == L'+') {
          plus = true;
        } else {
          break;
        }
        FORMAT_NEXT;
      }

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

      const char *string = NULL;
      const wchar_t *wstring = NULL;

      intmax_t number = 0;
      size_t number_precision = 1;
      char number_padding = '0';
      char number_sign = '\0';

      // Conversion specifiers.
      switch (FORMAT_NEXT) {
        case L'a': {
          // Abbreviated weekday name.
          if (timeptr->tm_wday >= 0 && timeptr->tm_wday < 7)
            wstring = lc_time->abday[timeptr->tm_wday];
          break;
        }
        case L'A': {
          // Full weekday name.
          if (timeptr->tm_wday >= 0 && timeptr->tm_wday < 7)
            wstring = lc_time->day[timeptr->tm_wday];
          break;
        }
        case L'b':
        case L'h': {
          // Abbreviated month name.
          if (timeptr->tm_mon >= 0 && timeptr->tm_mon < 12)
            wstring = lc_time->abmon[timeptr->tm_mon];
          break;
        }
        case L'B': {
          // Full month name.
          if (timeptr->tm_mon >= 0 && timeptr->tm_mon < 12)
            wstring = lc_time->mon[timeptr->tm_mon];
          break;
        }
        case L'c': {
          // Appropriate date and time representation.
          subformat = lc_time->d_t_fmt;
          continue;
        }
        case L'C': {
          // Century. Ensure that '-' is printed even if the year lies
          // between [-99,-1].
          number = (intmax_t)timeptr->tm_year + 1900;
          if (number < 0)
            number_sign = '-';
          number /= 100;
          number_precision = 2;
          break;
        }
        case L'd': {
          // Day of the month with leading zero.
          number = timeptr->tm_mday < 1
                       ? 1
                       : timeptr->tm_mday <= 31 ? timeptr->tm_mday : 31;
          number_precision = 2;
          break;
        }
        case L'D': {
          // Equivalent to "%m/%d/%y".
          subformat = L"%m/%d/%y";
          continue;
        }
        case L'e': {
          // Day of the month without leading zero.
          number = timeptr->tm_mday < 1
                       ? 1
                       : timeptr->tm_mday <= 31 ? timeptr->tm_mday : 31;
          number_precision = 2;
          number_padding = ' ';
          break;
        }
        case L'f': {
          // Extensions: nanoseconds.
          unsigned long value = timeptr->tm_nsec;
          unsigned long digit = 1000000000;
          size_t printed = 0;
          do {
            digit /= 10;
            PUTCHAR((value / digit) % 10 + '0');
            ++printed;
          } while (value % digit != 0);
          while (printed++ < field_width)
            PUTCHAR(left_padding);
          continue;
        }
        case L'F': {
          if (left_padding == ' ' && !plus && field_width == 0) {
            // Equivalent to "%+4Y-%m-%d" if no flag and no minimum field
            // width are specified.
            plus = true;
            field_width = 4;
          } else {
            // If a minimum field width of x is specified, the year shall
            // be output as if by the Y specifier with whatever flag was
            // given and a minimum field width of x-6. If x is less than
            // 6, the behavior shall be as if x equalled 6.
            field_width = field_width >= 6 ? field_width - 6 : 0;
          }
          subformat = L"-%m-%d";
          goto year_number;
        }
        case L'g': {
          // Last two digits of the week-based year number.
          number = (iso_8601_year(timeptr) + 1900) % 100;
          if (number < 0)
            number = -number;
          number_precision = 2;
          break;
        }
        case L'G': {
          // Week-based year number.
          number = (intmax_t)iso_8601_year(timeptr) + 1900;
          number_precision = 4;
          break;
        }
        case L'H': {
          // 24-hour based hour with leading zero.
          number = timeptr->tm_hour < 0
                       ? 0
                       : timeptr->tm_hour <= 23 ? timeptr->tm_hour : 23;
          number_precision = 2;
          break;
        }
        case L'I': {
          // 12-hour based hour with leading zero.
          number =
              timeptr->tm_hour <= 0
                  ? 12
                  : timeptr->tm_hour <= 12
                        ? timeptr->tm_hour
                        : timeptr->tm_hour <= 23 ? timeptr->tm_hour - 12 : 11;
          number_precision = 2;
          break;
        }
        case L'j': {
          // Day of the year with leading zero.
          number = timeptr->tm_yday < 0
                       ? 1
                       : timeptr->tm_yday <= 365 ? timeptr->tm_yday + 1 : 366;
          number_precision = 3;
          break;
        }
        case L'm': {
          // Month number with leading zero.
          number = timeptr->tm_mon < 0
                       ? 1
                       : timeptr->tm_mon <= 11 ? timeptr->tm_mon + 1 : 12;
          number_precision = 2;
          break;
        }
        case L'M': {
          // Minutes with leading zero.
          number = timeptr->tm_min < 0
                       ? 0
                       : timeptr->tm_min <= 59 ? timeptr->tm_min : 59;
          number_precision = 2;
          break;
        }
        case L'n': {
          // Newline.
          string = "\n";
          break;
        }
        case L'p': {
          // "a.m." / "p.m." string.
          wstring = timeptr->tm_hour < 12 ? lc_time->am_str : lc_time->pm_str;
          break;
        }
        case L'r': {
          // Time in a.m. and p.m. notation.
          subformat = lc_time->t_fmt_ampm;
          continue;
        }
        case L'R': {
          // Time in 24-hour notation.
          subformat = L"%H:%M";
          continue;
        }
        case L'S': {
          // Seconds with leading zero.
          number = timeptr->tm_sec < 0
                       ? 0
                       : timeptr->tm_sec <= 60 ? timeptr->tm_sec : 60;
          number_precision = 2;
          break;
        }
        case L't': {
          // Tab.
          string = "\t";
          break;
        }
        case L'T': {
          // Equivalent to "%H:%M:%S".
          subformat = L"%H:%M:%S";
          continue;
        }
        case L'u': {
          // Number of the weekday (Monday is 1, Sunday is 7).
          number = timeptr->tm_wday < 1 || timeptr->tm_wday > 6
                       ? 7
                       : timeptr->tm_wday;
          break;
        }
        case L'U': {
          // Number of the week (first Sunday starts week 1).
          number = (timeptr->tm_yday - timeptr->tm_wday + 7) / 7;
          number_precision = 2;
          break;
        }
        case L'V': {
          // Number of the week (ISO 8601).
          number = iso_8601_week(timeptr);
          number_precision = 2;
          break;
        }
        case L'W': {
          // Number of the week (first Monday starts week 1).
          number = (timeptr->tm_yday - (timeptr->tm_wday + 6) % 7 + 7) / 7;
          number_precision = 2;
          break;
        }
        case L'w': {
          // Number of the weekday (Sunday is 0, Saturday is 6).
          number = timeptr->tm_wday < 1 || timeptr->tm_wday > 6
                       ? 0
                       : timeptr->tm_wday;
          break;
        }
        case L'x': {
          // Appropriate date representation.
          subformat = lc_time->d_fmt;
          continue;
        }
        case L'X': {
          // Appropriate time representation.
          subformat = lc_time->t_fmt;
          continue;
        }
        case L'y': {
          // Last two digits of the year number. Ensure that this also
          // works for negative years.
          number = ((intmax_t)timeptr->tm_year + 1900) % 100;
          if (number < 0)
            number = -number;
          number_precision = 2;
          break;
        }
        year_number:
        case L'Y': {
          // Year number.
          number = (intmax_t)timeptr->tm_year + 1900;
          number_precision = 4;
          break;
        }
        case L'z': {
          // Timezone offset in the form ±hhmm.
          if (timeptr->tm_isdst < 0)
            continue;
          int minutes = timeptr->tm_gmtoff / 60;
          number = (minutes / 60) * 100 + minutes % 60;
          number_precision = 4;
          number_sign = '+';
          break;
        }
        case L'Z': {
          // Timezone name or abbreviation.
          string = timeptr->tm_zone;
          break;
        }
        case L'%': {
          // Ampersand.
          string = "%";
          break;
        }
      }

      if (string != NULL) {
        // ASCII string (e.g., timezone name). Print directly.
        while (*string != '\0')
          PUTCHAR(*string++);
      } else if (wstring != NULL) {
#if WIDE
        // Wide character string. Copy it literally.
        while (*wstring != '\0')
          PUTCHAR(*wstring++);
#else
        // Convert wide character string to multibyte.
        const struct lc_ctype *ctype = locale->ctype;
        while (*wstring != '\0') {
          char buf[MB_LEN_MAX];
          ssize_t len = ctype->c32tomb(buf, *wstring++, ctype->data);
          if (len < 0) {
            PUTCHAR('?');
          } else {
            for (size_t i = 0; i < (size_t)len; ++i)
              PUTCHAR(buf[i]);
          }
        }
#endif
      } else {
        // Number. Construct digits.
        uintmax_t value;
        if (number < 0) {
          number_sign = '-';
          plus = false;
          value = -number;
        } else {
          value = number;
        }
        char number_buf[sizeof(uintmax_t) * 3];
        char *number_ptr = number_buf + sizeof(number_buf);
        do {
          *--number_ptr = '0' + value % 10;
          value /= 10;
        } while (value > 0);
        size_t buflen = number_buf + sizeof(number_buf) - number_ptr;

        // Print sign. Add a plus in case the field width or the length
        // of the digits exceeds the standard precision of the number.
        if (plus &&
            (buflen > number_precision || field_width > number_precision))
          number_sign = '+';
        if (number_sign != '\0') {
          PUTCHAR(number_sign);
          if (field_width > 0)
            --field_width;
          // We shouldn't print spaces after + or -.
          left_padding = '0';
        }

        // Print minimum field width padding.
        while (field_width-- >
               (buflen > number_precision ? buflen : number_precision))
          PUTCHAR(left_padding);

        // Print number padding.
        while (number_precision-- > buflen)
          PUTCHAR(number_padding);

        // Print digits.
        while (number_ptr < number_buf + sizeof(number_buf))
          PUTCHAR(*number_ptr++);
      }
#undef FORMAT_PEEK
#undef FORMAT_NEXT
    } else if (*subformat != L'\0') {
#if WIDE
      // Copy wide character from subformat directly.
      PUTCHAR(*subformat);
#else
      // Convert to multibyte string first.
      const struct lc_ctype *ctype = locale->ctype;
      char buf[MB_LEN_MAX];
      ssize_t len = ctype->c32tomb(buf, *subformat, ctype->data);
      if (len < 0) {
        PUTCHAR('?');
      } else {
        for (size_t i = 0; i < (size_t)len; ++i)
          PUTCHAR(buf[i]);
      }
#endif
      ++subformat;
    } else {
      PUTCHAR(*format);
      ++format;
    }
  }
#undef PUTCHAR
  s[nstored] = '\0';
  return nstored;
}
