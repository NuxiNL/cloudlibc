// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/refcount.h>

#include <errno.h>
#include <locale.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

// Languages.

#include "messages/nl_nl.h"
#include "messages/ru_ru.h"

#include "monetary/en_us.h"
#include "monetary/nl_nl.h"
#include "monetary/ru_ru.h"

#include "numeric/en_us.h"
#include "numeric/nl_nl.h"
#include "numeric/ru_ru.h"

#include "time/en_us.h"
#include "time/fr_fr.h"
#include "time/nl_nl.h"
#include "time/ru_ru.h"
#include "time/zh_cn.h"

static struct {
  char name[5];
  const struct lc_collate *collate;
  const struct lc_messages *messages;
  const struct lc_monetary *monetary;
  const struct lc_numeric *numeric;
  const struct lc_time *time;
} languages[] = {
    // TODO(ed): Make these locales more complete.
    {"cs_CZ", &__collate_posix, &__messages_en_us, &__monetary_posix,
     &__numeric_posix, &time_en_us},
    {"en_US", &__collate_posix, &__messages_en_us, &monetary_en_us,
     &numeric_en_us, &time_en_us},
    {"fr_CA", &__collate_posix, &__messages_en_us, &__monetary_posix,
     &__numeric_posix, &time_fr_fr},
    {"fr_FR", &__collate_posix, &__messages_en_us, &__monetary_posix,
     &__numeric_posix, &time_fr_fr},
    {"nl_NL", &__collate_posix, &messages_nl_nl, &monetary_nl_nl,
     &numeric_nl_nl, &time_nl_nl},
    {"ru_RU", &__collate_posix, &messages_ru_ru, &monetary_ru_ru,
     &numeric_ru_ru, &time_ru_ru},
    {"zh_CN", &__collate_posix, &__messages_en_us, &__monetary_posix,
     &__numeric_posix, &time_zh_cn},
};

// Timezones.

#include "timezone/tzdata.h"

locale_t newlocale(int category_mask, const char *locale, locale_t base) {
  struct __locale new_locale = {};
  if (*locale == '\0' || strcmp(locale, "C") == 0 ||
      strcmp(locale, "POSIX") == 0) {
    // Inherit from the C locale.
    new_locale = *LC_C_LOCALE;
  } else {
    // In this implementation, a locale name has the form
    // "[language][.ctype][@timezone]". Examples of valid locale names
    // include "nl_NL.UTF-8@Europe/Amsterdam", "@Australia/Melbourne"
    // and "en_US.US-ASCII". First split the string into separate
    // components.
    const char *language = locale;
    const char *ctype = language + strcspn(language, ".@");
    const char *timezone = ctype + strcspn(ctype, "@");

    // Process the language name.
    if (language != ctype) {
      size_t len = ctype - language;
      if (len <= sizeof(languages[0].name)) {
        for (size_t i = 0; i < __arraycount(languages); ++i) {
          if (strncmp(language, languages[i].name, len) == 0 &&
              languages[i].name[len] == '\0') {
            new_locale.collate = languages[i].collate;
            new_locale.messages = languages[i].messages;
            new_locale.monetary = languages[i].monetary;
            new_locale.numeric = languages[i].numeric;
            new_locale.time = languages[i].time;
            break;
          }
        }
      }
    }

    // Process the character set name.
    if (*ctype++ == '.')
      new_locale.ctype = __lookup_ctype(ctype, timezone - ctype);

    // Process the timezone name.
    if (*timezone++ == '@') {
      size_t i = 0;
      const char *timezone_name = timezone_names;
      do {
        if (strcmp(timezone, timezone_name) == 0) {
          new_locale.timezone = &timezones[i];
          break;
        }
        ++i;
        timezone_name += strlen(timezone_name) + 1;
      } while (*timezone_name != '\0');
    }
  }

  // Inherit from the C locale in case no valid base locale is
  // specified.
  if (base == NULL)
    base = LC_C_LOCALE;

  // Roll back any categories that are not in the category mask.
  bool different = false;
#define PATCH_CATEGORY(bit, name)               \
  do {                                          \
    if ((category_mask & bit) == 0) {           \
      /* Use value from base locale instead. */ \
      new_locale.name = base->name;             \
    } else if (new_locale.name != base->name) { \
      different = true;                         \
      if (new_locale.name == NULL) {            \
        /* No value available. */               \
        errno = ENOENT;                         \
        return NULL;                            \
      }                                         \
    }                                           \
  } while (0)
  PATCH_CATEGORY(LC_COLLATE_MASK, collate);
  PATCH_CATEGORY(LC_CTYPE_MASK, ctype);
  PATCH_CATEGORY(LC_MESSAGES_MASK, messages);
  PATCH_CATEGORY(LC_MONETARY_MASK, monetary);
  PATCH_CATEGORY(LC_NUMERIC_MASK, numeric);
  PATCH_CATEGORY(LC_TIME_MASK, time);
  PATCH_CATEGORY(LC_TIMEZONE_MASK, timezone);
#undef PATCH_CATEGORY

  // New locale is not any different from the base locale. Simply
  // increase the reference count on the base locale.
  if (!different) {
    refcount_acquire(&base->refcount);
    return base;
  }

  // Return a copy of the new locale object.
  locale_t copy = malloc(sizeof(*copy));
  if (copy != NULL) {
    *copy = new_locale;
    refcount_init(&copy->refcount, 1);
    atomic_init(&copy->compiled, NULL);
  }
  return copy;
}
