// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <testing.h> - Unit testing

#ifndef _TESTING_H_
#define _TESTING_H_

#include <_/types.h>

// clang-format off

// Instantiate the same macro for all separate types.
#define _MACRO_FOREACH_INEQUALITY_TYPE(macro, ...) \
  macro(_Bool, bool, __VA_ARGS__)                  \
  macro(char, char, __VA_ARGS__)                   \
  macro(signed char, schar, __VA_ARGS__)           \
  macro(unsigned char, uchar, __VA_ARGS__)         \
  macro(short, short, __VA_ARGS__)                 \
  macro(unsigned short, ushort, __VA_ARGS__)       \
  macro(int, int, __VA_ARGS__)                     \
  macro(unsigned int, uint, __VA_ARGS__)           \
  macro(long, long, __VA_ARGS__)                   \
  macro(unsigned long, ulong, __VA_ARGS__)         \
  macro(long long, llong, __VA_ARGS__)             \
  macro(unsigned long long, ullong, __VA_ARGS__)   \
  macro(float, float, __VA_ARGS__)                 \
  macro(double, double, __VA_ARGS__)               \
  macro(long double, ldouble, __VA_ARGS__)         \
  macro(const void *, ptr, __VA_ARGS__)
#define _MACRO_FOREACH_TYPE(macro, ...)              \
  macro(float _Complex, cfloat, __VA_ARGS__)         \
  macro(double _Complex, cdouble, __VA_ARGS__)       \
  macro(long double _Complex, cldouble, __VA_ARGS__) \
  _MACRO_FOREACH_INEQUALITY_TYPE(macro, __VA_ARGS__)

// Evaluate to a different identifier name based on the type.
#define _EXPRESSION_FOREACH_CASE(type, stype, prefix) \
  type: prefix##_##stype,                             \
  type const: prefix##_##stype,                       \
  type volatile: prefix##_##stype,                    \
  type const volatile: prefix##_##stype,
#define _EXPRESSION_FOREACH_INEQUALITY_TYPE(prefix, expression)             \
  _Generic(expression,                                                      \
           _MACRO_FOREACH_INEQUALITY_TYPE(_EXPRESSION_FOREACH_CASE, prefix) \
           default: prefix##_ptr)
#define _EXPRESSION_FOREACH_TYPE(prefix, expression)             \
  _Generic(expression,                                           \
           _MACRO_FOREACH_TYPE(_EXPRESSION_FOREACH_CASE, prefix) \
           default: prefix##_ptr)

// clang-format on

// Test registration.
//
// Syntax:
//
//   TEST(module, name) {
//     ...
//   }
//
//   TEST_SEPARATE_PROCESS(module, name) {
//     ...
//   }

struct __test {
  const char *__name;
  void (*__func)(int);
  _Bool __separate_process;
};

#define TEST(module, name)                                               \
  static void __test_func_##module##_##name(int);                        \
  static struct __test __test_obj_##module##_##name __section("__tests") \
      __used = {#module "::" #name, __test_func_##module##_##name, 0};   \
  static void __test_func_##module##_##name(int fd_tmp)
#define TEST_SEPARATE_PROCESS(module, name)                              \
  static void __test_func_##module##_##name(int);                        \
  static struct __test __test_obj_##module##_##name __section("__tests") \
      __used = {#module "::" #name, __test_func_##module##_##name, 1};   \
  static void __test_func_##module##_##name(int fd_tmp)

// Test execution.
//
// Syntax:
//
//   int main(void) {
//     int logfile = openat(...);
//     int tmpdir = openat(...);
//     testing_execute(tmpdir, logfile, nthreads);
//     return 0;
//   }

__BEGIN_DECLS
void testing_execute(int, int, unsigned int);
__END_DECLS

// Numerical comparison.
//
// Syntax:
//
//   ASSERT_EQ(expected, actual);
//   ASSERT_GE(expected, actual);
//   ASSERT_GT(expected, actual);
//   ASSERT_LE(expected, actual);
//   ASSERT_LT(expected, actual);
//   ASSERT_NE(expected, actual);

#define _GENERATE_TEST_FAILED(type, stype, unused)                             \
  _Noreturn void __test_failed_##stype(const char *, type, const char *, type, \
                                       const char *, const char *, int);
__BEGIN_DECLS
_MACRO_FOREACH_TYPE(_GENERATE_TEST_FAILED, unused)
__END_DECLS
#undef _GENERATE_TEST_FAILED

#define _GENERATE_COMPARE(type, stype, suffix, op)                   \
  static __inline void __test_compare_##suffix##_##stype(            \
      type expected, const char *expected_str, type actual,          \
      const char *actual_str, const char *file, int line) {          \
    if (!(expected op actual))                                       \
      __test_failed_##stype(#suffix, expected, expected_str, actual, \
                            actual_str, file, line);                 \
  }
_MACRO_FOREACH_TYPE(_GENERATE_COMPARE, EQ, ==)
_MACRO_FOREACH_TYPE(_GENERATE_COMPARE, NE, !=)
_MACRO_FOREACH_INEQUALITY_TYPE(_GENERATE_COMPARE, GE, >=)
_MACRO_FOREACH_INEQUALITY_TYPE(_GENERATE_COMPARE, GT, >)
_MACRO_FOREACH_INEQUALITY_TYPE(_GENERATE_COMPARE, LE, <=)
_MACRO_FOREACH_INEQUALITY_TYPE(_GENERATE_COMPARE, LT, <)
#undef _GENERATE_COMPARE

#define ASSERT_EQ(expected, actual)                   \
  _EXPRESSION_FOREACH_TYPE(__test_compare_EQ, actual) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)
#define ASSERT_NE(expected, actual)                   \
  _EXPRESSION_FOREACH_TYPE(__test_compare_NE, actual) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)
#define ASSERT_GE(expected, actual)                              \
  _EXPRESSION_FOREACH_INEQUALITY_TYPE(__test_compare_GE, actual) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)
#define ASSERT_GT(expected, actual)                              \
  _EXPRESSION_FOREACH_INEQUALITY_TYPE(__test_compare_GT, actual) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)
#define ASSERT_LE(expected, actual)                              \
  _EXPRESSION_FOREACH_INEQUALITY_TYPE(__test_compare_LE, actual) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)
#define ASSERT_LT(expected, actual)                              \
  _EXPRESSION_FOREACH_INEQUALITY_TYPE(__test_compare_LT, actual) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)

// Boolean comparison.
//
// Syntax:
//
//   ASSERT_TRUE(expr);
//   ASSERT_FALSE(expr);

#define ASSERT_TRUE(actual) \
  __test_compare_EQ_bool(1, "true", !!(actual), #actual, __FILE__, __LINE__)
#define ASSERT_FALSE(actual) \
  __test_compare_EQ_bool(0, "false", !!(actual), #actual, __FILE__, __LINE__)

// Array comparison.
//
// Syntax:
//
//   ASSERT_ARREQ(expected, actual, length);

#define _GENERATE_COMPARE_ARREQ(type, stype, unused)                          \
  void __test_compare_ARREQ_##stype(type const *, const char *, type const *, \
                                    const char *, __size_t, const char *,     \
                                    int);
__BEGIN_DECLS
_MACRO_FOREACH_TYPE(_GENERATE_COMPARE_ARREQ, unused)
__END_DECLS
#undef _GENERATE_COMPARE_ARREQ

#define ASSERT_ARREQ(expected, actual, length)              \
  _EXPRESSION_FOREACH_TYPE(__test_compare_ARREQ, *(actual)) \
  (expected, #expected, actual, #actual, length, __FILE__, __LINE__)

// String comparison.
//
// Syntax:
//
//   ASSERT_STREQ(expected, actual);
//   ASSERT_STRNE(expected, actual);

#define _GENERATE_COMPARE_STR(type, stype, op)                      \
  void __test_compare_STR##op##_##stype(type const *, const char *, \
                                        type const *, const char *, \
                                        const char *, int);
__BEGIN_DECLS
_MACRO_FOREACH_TYPE(_GENERATE_COMPARE_STR, EQ)
_MACRO_FOREACH_TYPE(_GENERATE_COMPARE_STR, NE)
__END_DECLS
#undef _GENERATE_COMPARE_STREQ

#define ASSERT_STREQ(expected, actual)                      \
  _EXPRESSION_FOREACH_TYPE(__test_compare_STREQ, *(actual)) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)
#define ASSERT_STRNE(expected, actual)                      \
  _EXPRESSION_FOREACH_TYPE(__test_compare_STRNE, *(actual)) \
  (expected, #expected, actual, #actual, __FILE__, __LINE__)

// Notes: print additional values upon failed assertions.
//
// Syntax:
//
//   TEST(module, name) {
//     for (int i = 0; i < 100; ++i) {
//       SCOPED_NOTE(i, {
//         ...
//       });
//     }
//   }

enum __test_note_kind {
#define _TEST_NOTE_KIND(type, stype, unused) __test_note_kind_##stype,
  _MACRO_FOREACH_TYPE(_TEST_NOTE_KIND, unused)
#undef _TEST_NOTE_KIND
};

struct __test_note {
  enum __test_note_kind __kind;
  union {
#define _TEST_NOTE_VALUE(type, stype, unused) type __value_##stype;
    _MACRO_FOREACH_TYPE(_TEST_NOTE_VALUE, unused)
#undef _TEST_NOTE_VALUE
  };

  const char *__expression;
  const char *__file;
  int __line;

  const struct __test_note *__parent;
};

extern _Thread_local const struct __test_note *__test_note_stack;

#define _GENERATE_NOTE_SET(type, stype, unused)                       \
  static __inline void __note_set_##stype(struct __test_note *__note, \
                                          type __value) {             \
    __note->__kind = __test_note_kind_##stype;                        \
    __note->__value_##stype = __value;                                \
  }
_MACRO_FOREACH_TYPE(_GENERATE_NOTE_SET, unused)
#undef _GENERATE_NOTE_SET

#define SCOPED_NOTE(exp, block) _SCOPED_NOTE(exp, block, __COUNTER__)
#define _SCOPED_NOTE(exp, block, counter) __SCOPED_NOTE(exp, block, counter)
#define __SCOPED_NOTE(exp, block, counter)                             \
  do {                                                                 \
    struct __test_note __note_##counter;                               \
    _EXPRESSION_FOREACH_TYPE(__note_set, exp)(&__note_##counter, exp); \
    __note_##counter.__expression = #exp;                              \
    __note_##counter.__file = __FILE__;                                \
    __note_##counter.__line = __LINE__;                                \
    __note_##counter.__parent = __test_note_stack;                     \
    __test_note_stack = &__note_##counter;                             \
    do                                                                 \
      block while (0);                                                 \
    __test_note_stack = __note_##counter.__parent;                     \
  } while (0)

#endif
