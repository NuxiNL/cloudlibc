// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fmtmsg.h>
#include <stdbool.h>
#include <stdio.h>

int fmtmsg(long classification, const char *label, int severity,
           const char *text, const char *action, const char *tag) {
  // Prepare the exact string that needs to be printed in an array.
  const char *strings[12];
  for (size_t i = 0; i < __arraycount(strings); ++i)
    strings[i] = "";
  size_t nstrings = 0;
  bool start_of_line = true;

#define PUT_NEWLINE()             \
  do {                            \
    if (!start_of_line) {         \
      strings[nstrings++] = "\n"; \
      start_of_line = true;       \
    }                             \
  } while (0)
#define PUT_STRING_NOSPACE(str)  \
  do {                           \
    strings[nstrings++] = (str); \
    start_of_line = false;       \
  } while (0)
#define PUT_STRING_SPACE(str)    \
  do {                           \
    if (!start_of_line)          \
      strings[nstrings++] = " "; \
    strings[nstrings++] = (str); \
    start_of_line = false;       \
  } while (0)

  // First line: label, severity and text.
  if (label != MM_NULLLBL) {
    PUT_STRING_NOSPACE(label);
    PUT_STRING_NOSPACE(":");
  }
  switch (severity) {
    case MM_HALT:
      PUT_STRING_SPACE("HALT:");
      break;
    case MM_ERROR:
      PUT_STRING_SPACE("ERROR:");
      break;
    case MM_WARNING:
      PUT_STRING_SPACE("WARNING:");
      break;
    case MM_INFO:
      PUT_STRING_SPACE("INFO:");
      break;
  }
  if (text != MM_NULLTXT)
    PUT_STRING_SPACE(text);
  PUT_NEWLINE();

  // Second line: action and tag.
  if (action != MM_NULLACT) {
    PUT_STRING_NOSPACE("TO FIX: ");
    PUT_STRING_NOSPACE(action);
  }
  if (tag != MM_NULLTAG)
    PUT_STRING_SPACE(tag);
  PUT_NEWLINE();

#undef PUT_NEWLINE
#undef PUT_STRING_NOSPACE
#undef PUT_STRING_SPACE

  // Print all of the prepared strings.
  return fprintf(stderr, "%s%s%s%s%s%s%s%s%s%s%s%s", strings[0], strings[1],
                 strings[2], strings[3], strings[4], strings[5], strings[6],
                 strings[7], strings[8], strings[9], strings[10],
                 strings[11]) == -1
             ? MM_NOTOK
             : MM_OK;
}
