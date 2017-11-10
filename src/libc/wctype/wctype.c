// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <wctype.h>

wctype_t wctype(const char *property) {
#define PROPERTY(name)              \
  if (strcmp(property, #name) == 0) \
    return isw##name;
  PROPERTY(alnum);
  PROPERTY(alpha);
  PROPERTY(blank);
  PROPERTY(cntrl);
  PROPERTY(digit);
  PROPERTY(graph);
  PROPERTY(lower);
  PROPERTY(print);
  PROPERTY(punct);
  PROPERTY(space);
  PROPERTY(upper);
  PROPERTY(xdigit);
#undef PROPERTY
  return NULL;
}
