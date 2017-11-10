#!/usr/bin/env python

# Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
#
# SPDX-License-Identifier: BSD-2-Clause

import datetime

DATAFILES = set(['africa', 'antarctica', 'asia', 'australasia',
                 'backward', 'etcetera', 'europe', 'northamerica',
                 'pacificnew', 'southamerica'])

RULES = {}
ERAS = {}
LINKS = {}

# Standard ruleset for countries that have fixed daylight saving time amounts.
RULES['rules__'] = []
RULES['rules_020'] = [{
    'year_from': 0,
    'year_to': 255,
    'month': 0,
    'monthday': 1,
    'weekday': 7,
    'hour': 0,
    'minute': 0,
    'timebase': 'TIMEBASE_CUR',
    'save': 20,
    'abbreviation': '',
}]
RULES['rules_030'] = [{
    'year_from': 0,
    'year_to': 255,
    'month': 0,
    'monthday': 1,
    'weekday': 7,
    'hour': 0,
    'minute': 0,
    'timebase': 'TIMEBASE_CUR',
    'save': 30,
    'abbreviation': '',
}]
RULES['rules_100'] = [{
    'year_from': 0,
    'year_to': 255,
    'month': 0,
    'monthday': 1,
    'weekday': 7,
    'hour': 0,
    'minute': 0,
    'timebase': 'TIMEBASE_CUR',
    'save': 60,
    'abbreviation': '',
}]

def get_eras_name(tz):
  return 'eras_' + tz.replace('/', '_').replace('+', '').replace('-', '_')

def get_rules_name(tz):
  return 'rules_' + tz.replace(':', '').replace('-', '_')

def handle_zoneline(ename, fields, end, timebase):
  entry = {}
  # Timezone offset.
  if fields[0][0] == '-':
    entry['gmtoff'] = -1
    fields[0] = fields[0][1:]
  else:
    entry['gmtoff'] = 1
  gmtoff = fields[0].split(':') + ['0', '0']
  entry['gmtoff'] *= (int(gmtoff[0]) * 3600 + int(gmtoff[1]) * 60 +
                      int(gmtoff[2]))

  # Daylight saving time rules.
  entry['rules'] = get_rules_name(fields[1])

  # Abbreviation.
  abbr = fields[2].split('/') + ['']
  entry['abbreviation_std'] = abbr[0]
  entry['abbreviation_dst'] = abbr[1]

  # End date.
  entry['end'] = end
  entry['timebase'] = timebase
  if ename in ERAS:
    ERAS[ename].append(entry)
  else:
    ERAS[ename] = [entry]

def is_leap(year):
  year %= 400
  return (year % 4 == 0 and year % 100 != 0) or year == 100

def get_ruleline_day(daystr, year_from, year_to, month):
  try:
    # Exact day number.
    return int(daystr), 7
  except ValueError:
    daymap = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat']
    greater_than = daystr.split('>=')
    if len(greater_than) == 2:
      # Weekday greater than n, e.g. "Sun>=16".
      return int(greater_than[1]), daymap.index(greater_than[0])

    # Last weekday of the month, e.g. "lastSun".
    assert daystr[:4] == 'last'
    weekday = daymap.index(daystr[4:])

    # Safety belt: make sure the rule does not take effect on February
    # 29th. This code does not support this yet.
    if month == 1:
      for year in range(year_from, year_to + 1):
        day = 29 if is_leap(year) else 28
        while True:
          d = datetime.date(year + 1900, month + 1, day)
          if d.isoweekday() - 1 == weekday:
            break
          day = day - 1
        assert day != 29

    # Translate it to "Day>=n".
    month_lengths = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    return month_lengths[month] - 6, weekday

def handle_ruleline(rname, fields):
  entry = {}

  # Year fields.
  entry['year_from'] = int(fields[0]) - 1900
  if fields[1] == 'max':
    entry['year_to'] = 255
  elif fields[1] == 'only':
    entry['year_to'] = entry['year_from']
  else:
    entry['year_to'] = int(fields[1]) - 1900

  # Unused type field.
  assert fields[2] == '-'

  # Month.
  entry['month'] = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul',
                    'Aug', 'Sep', 'Oct', 'Nov', 'Dec'].index(fields[3])

  # Day of the month.
  entry['monthday'], entry['weekday'] = get_ruleline_day(
      fields[4], entry['year_from'], entry['year_to'], entry['month'])

  # Time.
  time = fields[5]
  if time == '0':
    # Shorthand for 0:00.
    time = '0:00'
  if time[-1] == 's':
    entry['timebase'] = 'TIMEBASE_STD'
    time = time[:-1]
  elif time[-1] == 'u':
    entry['timebase'] = 'TIMEBASE_UTC'
    time = time[:-1]
  else:
    entry['timebase'] = 'TIMEBASE_CUR'
  time = time.split(':')
  entry['hour'] = int(time[0])
  assert entry['hour'] >= 0 and entry['hour'] <= 24
  entry['minute'] = int(time[1])
  assert entry['hour'] >= 0 and entry['hour'] < 60

  # Hack: Wrap entries for the start of the year to the year before. The
  # conversion algorithm always limits itself to entries of the year
  # corresponding with the standard time. This means that entries at the
  # start of January 1st may not be taken into account.
  if (entry['month'] == 0 and entry['monthday'] == 1 and
      entry['hour'] == 0 and entry['minute'] == 0):
   entry['year_from'] -= 1
   if entry['year_to'] < 255:
     entry['year_to'] -= 1
   entry['month'] = 11
   entry['monthday'] = 31
   entry['hour'] = 24
   entry['minute'] = 0

  # Amount of daylight saving.
  save = fields[6].split(':') + ['0']
  entry['save'] = int(save[0]) * 60 + int(save[1])

  entry['abbreviation'] = fields[7] if fields[7] != '-' else ''
  if rname in RULES:
    RULES[rname].append(entry)
  else:
    RULES[rname] = [entry]

def translate_day(fields):
  return fields[2]

def handle_endtime(fields):
  # Last record without an end time.
  if len(fields) == 0:
    return 0, 'u'

  # Workaround: In a couple of places, directives like 'lastSun' and
  # 'Sun>=1' are being used, even in Zone directives. Translate these to
  # exact dates.
  if len(fields) > 2:
    daymap = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
    daystr = fields[2]
    greater_than = daystr.split('>=')
    if len(greater_than) == 2:
      # "Sun>=1". Start at the specified date and increment until the
      # constraint has been met.
      weekday = daymap.index(greater_than[0])
      d = datetime.datetime.strptime(
          ' '.join((fields[0], fields[1], greater_than[1])), '%Y %b %d')
      while d.weekday() != weekday:
        d = d + datetime.timedelta(days=1)
      fields[2] = str(d.day)
    elif daystr.startswith('last'):
      # "lastSun". Start at the last day of the next month and decrement
      # until the constraint has been met.
      d = datetime.datetime.strptime(' '.join(fields[:2]), '%Y %b')
      d = datetime.datetime(d.year, d.month + 1, 1)
      weekday = daymap.index(daystr[4:])
      while True:
        d = d - datetime.timedelta(days=1)
        if d.weekday() == weekday:
          break
      fields[2] = str(d.day)

  timebase = 'c'
  if len(fields) == 1:
    dt = datetime.datetime.strptime(' '.join(fields), '%Y')
  elif len(fields) == 2:
    dt = datetime.datetime.strptime(' '.join(fields), '%Y %b')
  elif len(fields) == 3:
    fields[2] = translate_day(fields)
    dt = datetime.datetime.strptime(' '.join(fields), '%Y %b %d')
  elif len(fields) == 4:
    fields[2] = translate_day(fields[:3])

    # Timebase flag at the end of time.
    if fields[3][-1] == 's' or fields[3][-1] == 'u':
      timebase = fields[3][-1]
      fields[3] = fields[3][:-1]

    while True:
      try:
        dt = datetime.datetime.strptime(' '.join(fields), '%Y %b %d %H:%M:%S')
      except ValueError:
        if fields[3] == '24:00':
          # Workaround: Time value of 24:00. Translate it to 0:00:00.
          fields[2] = str(int(fields[2]) + 1)
          fields[3] = '0:00:00'
        else:
          # Add missing minutes/seconds.
          fields[3] = fields[3] + ':00'
      else:
        break
  return int((dt - datetime.datetime(1970, 1, 1)).total_seconds()), timebase

# Parse the data files.
for datafile in DATAFILES:
  with open(datafile, 'r') as tzfile:
    last_zone = None
    for line in tzfile:
      # Ignore trailing comments and empty lines.
      line = line.split('#')[0].strip()
      if not line:
        continue
      fields = line.split()

      if fields[0] == 'Rule':
        handle_ruleline(get_rules_name(fields[1]), fields[2:])
      elif fields[0] == 'Link':
        LINKS[fields[2]] = get_eras_name(fields[1])
      elif fields[0] == 'Zone':
        ename = get_eras_name(fields[1])
        LINKS[fields[1]] = ename
        last_zone = ename
        endtime, endtime_timebase = handle_endtime(fields[5:])
        handle_zoneline(ename, fields[2:], endtime, endtime_timebase)
      else:
        assert endtime != None
        endtime, endtime_timebase = handle_endtime(fields[3:])
        handle_zoneline(last_zone, fields, endtime, endtime_timebase)

def sort_ruleset(ruleset):
  return sorted(ruleset, key=lambda rule: (rule['month'], rule['monthday']))

# Postprocessing: fix up eras->end to be either relative to UTC, the
# local time or the standard time.
def fixup_era(era):
  # Time should be relative to local time. This is annoying, as it
  # requires us to do an actual simulation of the ruleset. Below is a
  # rough implementation of localtime_l().
  rules = RULES[era['rules']]
  save = 0

  # Convert end time to standard time if it's UTC.
  if era['timebase'] == 'u':
    era['end'] += era['gmtoff']
    era['timebase'] = 's'

  if len(rules) > 0:
    # Step 1: Determine ruleset of the previous year.
    lastsec = era['end'] - 1
    std = (datetime.datetime(1970, 1, 1) +
           datetime.timedelta(seconds=lastsec - era['gmtoff']))
    last_year = rules[0]['year_from']
    ruleset = []
    while len(rules) != 0 and rules[0]['year_from'] < std.year - 1900:
      if last_year != rules[0]['year_from']:
        ruleset = [rule for rule in ruleset
                        if rule['year_to'] >= rules[0]['year_from']]
      ruleset = sort_ruleset(ruleset + [rules[0]])
      # Determine which rule was active at the end of the year.
      last = None
      for rule in reversed(ruleset):
        if last == None:
          last = rule
        else:
          if last['year_to'] >= std.year - 1901:
            break
          if last['year_to'] < rule['year_to']:
            last = rule
        save = last['save'] * 60
      rules = rules[1:]

    # Step 2: Determine ruleset of the current year.
    ruleset = [rule for rule in ruleset
                    if rule['year_to'] >= std.year - 1900]
    while len(rules) != 0 and rules[0]['year_from'] == std.year - 1900:
      ruleset.append(rules[0])
      rules = rules[1:]
    ruleset = sort_ruleset(ruleset)

    # Step 3: determine whether there is a rule for the current year
    # that matches the time.
    for rule in ruleset:
      if rule['timebase'] == 'TIMEBASE_CUR':
        time = (datetime.datetime(1970, 1, 1) +
                datetime.timedelta(seconds=lastsec))
      elif rule['timebase'] == 'TIMEBASE_STD':
        time = (datetime.datetime(1970, 1, 1) +
                datetime.timedelta(seconds=lastsec - save))
      else:
        assert rule['timebase'] == 'TIMEBASE_UTC'
        time = (datetime.datetime(1970, 1, 1) +
                datetime.timedelta(seconds=lastsec - era['gmtoff'] - save))
      monthdaydelta = 0
      if rule['weekday'] != 7:
        monthdaydelta = (rule['weekday'] - time.isoweekday() +
                         time.day - rule['monthday']) % 7
      assert monthdaydelta >= 0 and monthdaydelta < 7
      if (rule['month'], rule['monthday'] + monthdaydelta,
          rule['hour'], rule['minute']) > (time.month - 1, time.day,
          time.hour, time.minute):
        break
      save = rule['save'] * 60

  era['end_save'] = save
  if era['timebase'] == 's':
    # Time should be relative to standard time.
    era['end'] -= era['gmtoff']
  else:
    assert era['timebase'] == 'c'
    era['end'] -= era['gmtoff'] + save

  return era

for name in RULES:
  RULES[name] = sorted(
      RULES[name], key=lambda r: (r['year_from'], r['month'], r['monthday']))

for name in ERAS:
  ERAS[name] = [fixup_era(era) for era in ERAS[name]]

# Print the timezone rules.
for name, rules in sorted(RULES.iteritems()):
  if len(rules) > 0:
    print 'static const struct lc_timezone_rule %s[] = {' % name
    for rule in rules:
      assert rule['save'] in [0, 20, 30, 60, 90, 120]
      print '    {%d, %d, %d, %d, %d, %d, %s, %d, \"%s\\0\"},' % (
          rule['year_from'], rule['year_to'], rule['month'], rule['weekday'],
          rule['monthday'], rule['hour'] * 60 + rule['minute'],
          rule['timebase'], rule['save'] / 10, rule['abbreviation'])
    print '};'

# Print the timezone eras.
for name, eras in sorted(ERAS.iteritems()):
  print 'static const struct lc_timezone_era %s[] = {' % name
  for era in eras:
    print '    {%s, %d, %d, %d, %d, \"%s\\0\", \"%s\\0\"},' % (
         'NULL' if len(RULES[era['rules']]) == 0 else era['rules'],
         len(RULES[era['rules']]), era['gmtoff'], era['end'],
         era['end_save'] / 600,
         era['abbreviation_std'], era['abbreviation_dst'])
  print '};'

# Print the links to the timezones.
print 'static const char timezone_names[] =';
for link in sorted(LINKS):
  print '    \"%s\\0\"' % link
print ';'

print 'static const struct lc_timezone timezones[] = {'
for name, ename in sorted(LINKS.iteritems()):
  assert len(ERAS[ename]) > 0
  print '    {%s, %d},' % (ename, len(ERAS[ename]))
print '};'
