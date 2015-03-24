// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <testing.h>
#include <tgmath.h>
#include <threads.h>
#include <wchar.h>

#include "testing_impl.h"

//
// Notes.
//

thread_local const struct __test_note *__test_note_stack = NULL;

//
// Value printing.
//

#define GENERATE_PRINT_VALUE_INT(type, stype, size, decimal)                \
  static void print_value_##stype(type value) {                             \
    __testing_printf("%10" size decimal " == %#10" size "x", value, value); \
    if (value >= ' ' && value <= '~')                                       \
      __testing_printf(" == '%c'", (char)value);                            \
  }

#if __CHAR_UNSIGNED__
GENERATE_PRINT_VALUE_INT(char, char, "hh", "u");
#else
GENERATE_PRINT_VALUE_INT(char, char, "hh", "d");
#endif
GENERATE_PRINT_VALUE_INT(signed char, schar, "hh", "d");
GENERATE_PRINT_VALUE_INT(unsigned char, uchar, "hh", "u");
GENERATE_PRINT_VALUE_INT(short, short, "h", "d");
GENERATE_PRINT_VALUE_INT(unsigned short, ushort, "h", "u");
GENERATE_PRINT_VALUE_INT(int, int, "", "d");
GENERATE_PRINT_VALUE_INT(unsigned int, uint, "", "u");
GENERATE_PRINT_VALUE_INT(long, long, "l", "d");
GENERATE_PRINT_VALUE_INT(unsigned long, ulong, "l", "u");
GENERATE_PRINT_VALUE_INT(long long, llong, "ll", "d");
GENERATE_PRINT_VALUE_INT(unsigned long long, ullong, "ll", "u");

#undef GENERATE_PRINT_VALUE_INT

#define GENERATE_PRINT_VALUE_FLOAT(type, stype, size)                         \
  static void print_value_##stype(type value) {                               \
    __testing_printf("%10" size "f == %#10" size "a", value, value);          \
  }                                                                           \
  static void print_value_c##stype(type complex value) {                      \
    __testing_printf("%10" size "f + %10" size "fi == %#10" size              \
                     "a + %#10" size "ai",                                    \
                     creal(value), cimag(value), creal(value), cimag(value)); \
  }

GENERATE_PRINT_VALUE_FLOAT(float, float, "");
GENERATE_PRINT_VALUE_FLOAT(double, double, "");
GENERATE_PRINT_VALUE_FLOAT(double, ldouble, "L");

#undef GENERATE_PRINT_VALUE_FLOAT

static void print_value_bool(_Bool value) {
  __testing_printf("%-5s", value ? "true" : "false");
}

static void print_value_ptr(const void *value) {
  __testing_printf("%p", value);
}

//
// Test failure.
//

static noreturn void print_test_footer(const char *file, int line) {
  // Print location of the failing test.
  __testing_printf(
      "Location:  %s:%d\n"
      "Errno:     %d, %s\n",
      file, line, errno, strerror(errno));

  // Print notes that are associated with this test.
  const struct __test_note *note = __test_note_stack;
  while (note != NULL) {
    __testing_printf(
        "--\n"
        "Note:      ");
    switch (note->kind) {
#define PRINT_NOTE(type, stype)               \
  case __test_note_kind_##stype:              \
    print_value_##stype(note->value_##stype); \
    break;
      _MACRO_FOREACH_TYPE(PRINT_NOTE)
#undef PRINT_NOTE
    }
    __testing_printf(
        " == (%s)\n"
        "Location:  %s:%d\n",
        note->expression, note->file, note->line);
    note = note->parent;
  }

  // Terminate.
  abort();
}

#define GENERATE_TEST_FAILED(type, stype)                                   \
  noreturn void __test_failed_##stype(                                      \
      const char *op, type expected, const char *expected_str, type actual, \
      const char *actual_str, const char *file, int line) {                 \
    __testing_printf(                                                       \
        "Test failed\n"                                                     \
        "--\n"                                                              \
        "Statement: ASSERT_%s(%s, %s)\n"                                    \
        "Expected:  ",                                                      \
        op, expected_str, actual_str);                                      \
    print_value_##stype(expected);                                          \
    __testing_printf(                                                       \
        " == (%s)\n"                                                        \
        "Actual:    ",                                                      \
        expected_str);                                                      \
    print_value_##stype(actual);                                            \
    __testing_printf(" == (%s)\n", actual_str);                             \
    print_test_footer(file, line);                                          \
  }
_MACRO_FOREACH_TYPE(GENERATE_TEST_FAILED)
#undef GENERATE_TEST_FAILED

#define GENERATE_COMPARE_ARREQ(type, stype)                                \
  void __test_compare_ARREQ_##stype(                                       \
      type const *expected, const char *expected_str, type const *actual,  \
      const char *actual_str, size_t length, const char *file, int line) { \
    for (size_t offset = 0; offset < length; ++offset) {                   \
      if (expected[offset] != actual[offset]) {                            \
        __testing_printf(                                                  \
            "Test failed\n"                                                \
            "--\n"                                                         \
            "Statement: ASSERT_ARREQ(%s, %s, %zu)\n"                       \
            "Expected:  ",                                                 \
            expected_str, actual_str, length);                             \
        print_value_##stype(expected[offset]);                             \
        __testing_printf(                                                  \
            " == (%s)[%zu]\n"                                              \
            "Actual:    ",                                                 \
            expected_str, offset);                                         \
        print_value_##stype(actual[offset]);                               \
        __testing_printf(" == (%s)[%zu]\n", actual_str, offset);           \
        print_test_footer(file, line);                                     \
      }                                                                    \
    }                                                                      \
  }
_MACRO_FOREACH_TYPE(GENERATE_COMPARE_ARREQ)
#undef GENERATE_COMPARE_ARREQ

#define GENERATE_COMPARE_STREQ(type, stype)                               \
  void __test_compare_STREQ_##stype(                                      \
      type const *expected, const char *expected_str, type const *actual, \
      const char *actual_str, const char *file, int line) {               \
    for (size_t offset = 0; expected[offset] != 0 || actual[offset] != 0; \
         ++offset) {                                                      \
      if (expected[offset] != actual[offset]) {                           \
        __testing_printf(                                                 \
            "Test failed\n"                                               \
            "--\n"                                                        \
            "Statement: ASSERT_STREQ(%s, %s)\n"                           \
            "Expected:  ",                                                \
            expected_str, actual_str);                                    \
        print_value_##stype(expected[offset]);                            \
        __testing_printf(                                                 \
            " == (%s)[%zu]\n"                                             \
            "Actual:    ",                                                \
            expected_str, offset);                                        \
        print_value_##stype(actual[offset]);                              \
        __testing_printf(" == (%s)[%zu]\n", actual_str, offset);          \
        print_test_footer(file, line);                                    \
      }                                                                   \
    }                                                                     \
  }
_MACRO_FOREACH_TYPE(GENERATE_COMPARE_STREQ)
#undef GENERATE_COMPARE_STREQ
