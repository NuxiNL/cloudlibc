#!/usr/bin/env python

# Copyright (c) 2015 Nuxi, https://nuxi.nl/
#
# SPDX-License-Identifier: BSD-2-Clause

import datetime
import sys

last_t2 = None
last_isdst = 0

def printline(t1, t2, isdst, abbreviation, consistency):
  ts1 = int((t1 - datetime.datetime(1970, 1, 1)).total_seconds())
  print 'TESTVECTOR(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \"%s\", %s)' % (
    ts1, t2.year, t2.month, t2.isoweekday() % 7, t2.day, t2.hour, t2.minute,
    t2.second, isdst, (t2 - t1).total_seconds(), abbreviation, consistency)

for line in sys.stdin:
  fields = line.split()

  # UTC time.
  try:
    t1 = datetime.datetime.strptime(' '.join(fields[2:6]), '%b %d %H:%M:%S %Y')
  except ValueError:
    continue

  # Local time.
  t2 = datetime.datetime.strptime(' '.join(fields[9:13]), '%b %d %H:%M:%S %Y')
  abbreviation = fields[13]
  isdst = int(fields[14][6:])

  if not last_t2 or last_t2 < t2:
    # mktime() will parse it back, even if isdst is set to -1.
    printline(t1, t2, isdst, abbreviation, 'BIDI')
  else:
    if last_isdst and not isdst:
      # mktime() will parse it back, but not when isdst is set to -1.
      consistency = 'HINT'
    else:
      # Time is ambiguous.
      consistency = 'UNI'

    # Print a line for the first moment in time that is inconsistent.
    printline(t1, t2, isdst, abbreviation, consistency)

    # Print a line for the last moment in time that is inconsistent.
    delta = last_t2 - t2
    printline(t1 + delta, t2 + delta, isdst, abbreviation, consistency)

    # Print a line for the first moment in time that is consistent again.
    s = datetime.timedelta(seconds=1)
    printline(t1 + delta + s, t2 + delta + s, isdst, abbreviation, 'BIDI')

  last_t2 = t2
  last_isdst = isdst
