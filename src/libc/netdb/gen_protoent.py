#!/usr/bin/env python

# Copyright (c) 2015 Nuxi, https://nuxi.nl/
#
# SPDX-License-Identifier: BSD-2-Clause

import collections
import csv

names = {}
aliases = collections.defaultdict(set)

# Add entries from IANA dataset.
with open('protocol-numbers-1.csv', 'r') as ufile:
  entries = csv.reader(ufile)
  entries.next()
  for entry in entries:
    if not entry[1]:
      continue
    number = int(entry[0])
    name = entry[1].lower().replace(' ', '-')
    if name.endswith('-(deprecated)'):
        name = name[:-13]
    if number in names:
      aliases[number].add(name)
    else:
      names[number] = name

# Add "ip" and "raw" as well, to match IPPROTO_IP and IPPROTO_RAW.
aliases[0].add('ip')
aliases[255].add('raw')

for proto, proto_aliases in sorted(aliases.iteritems()):
  print 'static char const *const aliases_%s[] = {' % proto
  for alias in proto_aliases:
    print '"%s",' % alias
  print 'NULL,'
  print '};'

print 'static const char *const aliases_none[] = {NULL};'

print 'static const struct protoent entries[] = {'
for proto, proto_name in sorted(names.iteritems()):
  print '{.p_name = (char *)"%s",' % proto_name
  print '.p_aliases = (char **)&aliases_%s,' % (
      proto if proto in aliases else "none")
  print '.p_proto = %d,},' % proto
print '};'
