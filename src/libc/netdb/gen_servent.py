#!/usr/bin/env python

# Copyright (c) 2015 Nuxi, https://nuxi.nl/
#
# SPDX-License-Identifier: BSD-2-Clause

import collections
import re
import xml.etree.ElementTree as ElementTree

NAMESPACE = '{http://www.iana.org/assignments}'

entries = collections.defaultdict(list)
protocols = collections.defaultdict(lambda: collections.defaultdict(int))

dom = ElementTree.parse('service-names-port-numbers.xml')
last_port = 1
for record in dom.getroot().iter(NAMESPACE + 'record'):
  try:
    protocol = record.find(NAMESPACE + 'protocol').text
    try:
      port = int(record.find(NAMESPACE + 'number').text)
    except ValueError:
      # Entries that use port ranges.
      continue
    name = record.find(NAMESPACE + 'name').text
  except AttributeError:
    # Entries without names, protocols, ports.
    continue

  #print '%s %s %s' % (name, protocol, number)
  assert last_port <= port
  assert port > 0 and port < 65536
  if name not in entries[port]:
    entries[port].append(name)
  protocols[port][name] |= {'tcp': 1, 'udp': 2, 'sctp': 4, 'dccp': 8}[protocol]

print 'const char __iana_port_numbers[] ='
for port, names in sorted(entries.iteritems()):
  for name in names:
    assert len(name) < 64
    print '"\\%03o\\%03o\\%03o%s\\000"' % (
        port / 256, port % 256, protocols[port][name], name)
print '"\\000\\000";'
