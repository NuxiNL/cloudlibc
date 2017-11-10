// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <langinfo.h>

char *nl_langinfo(nl_item item) {
  // Return fixed data for the C locale to prevent pulling in wide
  // character string conversion routines and malloc().
  switch (item) {
    case CODESET:
      return (char *)"US-ASCII";
    case D_T_FMT:
      return (char *)"%a %b %e %H:%M:%S %Y";
    case D_FMT:
      return (char *)"%m/%d/%y";
    case T_FMT:
      return (char *)"%H:%M:%S";
    case T_FMT_AMPM:
      return (char *)"%I:%M:%S %p";
    case AM_STR:
      return (char *)"AM";
    case PM_STR:
      return (char *)"PM";
    case DAY_1:
      return (char *)"Sunday";
    case DAY_2:
      return (char *)"Monday";
    case DAY_3:
      return (char *)"Tuesday";
    case DAY_4:
      return (char *)"Wednesday";
    case DAY_5:
      return (char *)"Thursday";
    case DAY_6:
      return (char *)"Friday";
    case DAY_7:
      return (char *)"Saturday";
    case ABDAY_1:
      return (char *)"Sun";
    case ABDAY_2:
      return (char *)"Mon";
    case ABDAY_3:
      return (char *)"Tue";
    case ABDAY_4:
      return (char *)"Wed";
    case ABDAY_5:
      return (char *)"Thu";
    case ABDAY_6:
      return (char *)"Fri";
    case ABDAY_7:
      return (char *)"Sat";
    case MON_1:
      return (char *)"January";
    case MON_2:
      return (char *)"February";
    case MON_3:
      return (char *)"March";
    case MON_4:
      return (char *)"April";
    case MON_5:
      return (char *)"May";
    case MON_6:
      return (char *)"June";
    case MON_7:
      return (char *)"July";
    case MON_8:
      return (char *)"August";
    case MON_9:
      return (char *)"September";
    case MON_10:
      return (char *)"October";
    case MON_11:
      return (char *)"November";
    case MON_12:
      return (char *)"December";
    case ABMON_1:
      return (char *)"Jan";
    case ABMON_2:
      return (char *)"Feb";
    case ABMON_3:
      return (char *)"Mar";
    case ABMON_4:
      return (char *)"Apr";
    case ABMON_5:
      return (char *)"May";
    case ABMON_6:
      return (char *)"Jun";
    case ABMON_7:
      return (char *)"Jul";
    case ABMON_8:
      return (char *)"Aug";
    case ABMON_9:
      return (char *)"Sep";
    case ABMON_10:
      return (char *)"Oct";
    case ABMON_11:
      return (char *)"Nov";
    case ABMON_12:
      return (char *)"Dec";
    case RADIXCHAR:
      return (char *)".";
    case YESEXPR:
      return (char *)"^[yY]";
    case NOEXPR:
      return (char *)"^[nN]";
    default:
      return (char *)"";
  }
}
