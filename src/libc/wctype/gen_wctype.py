#!/usr/bin/env python

# Copyright (c) 2015 Nuxi, https://nuxi.nl/
#
# SPDX-License-Identifier: BSD-2-Clause

import csv

ALNUM = set()
BLANK = set()
CNTRL = set()
DIGIT = set()
GRAPH = set()
LOWER = set()
PRINT = set()
SPACE = set()
UPPER = set()
XDIGIT = set()

TOLOWER = {}
TOUPPER = {}

WIDTH = {0: 0}

with open('UnicodeData.txt', 'r') as ufile:
  entries = csv.reader(ufile, delimiter=';')
  for entry in entries:
    codepoint = int(entry[0], 16)

    # alnum.
    if (entry[2][0] in ['L', 'N'] or
        (entry[2] == 'So' and ' LETTER ' in entry[1])) or codepoint == 0x345:
      ALNUM.add(codepoint)

    # blank.
    if (codepoint == ord('\t') or
        (entry[2] == 'Zs' and '<noBreak>' not in entry[5])):
      BLANK.add(codepoint)

    # cntrl and print.
    if entry[1] == '<control>' or entry[2] in ['Zl', 'Zp']:
      CNTRL.add(codepoint)
    else:
      PRINT.add(codepoint)
      WIDTH[codepoint] = 0 if entry[2] in ['Me', 'Mn', 'Cf'] else 1

    # digit and xdigit.
    if codepoint >= ord('0') and codepoint <= ord('9'):
      DIGIT.add(codepoint)
      XDIGIT.add(codepoint)
    elif ((codepoint >= ord('a') and codepoint <= ord('f')) or
          (codepoint >= ord('A') and codepoint <= ord('F'))):
      XDIGIT.add(codepoint)

    # lower.
    if entry[12]:
      LOWER.add(codepoint)
      TOUPPER[codepoint] = int(entry[12], 16) - codepoint

    # space and graph.
    if ((codepoint >= 0x9 and codepoint <= 0xd) or codepoint == 0x20 or
        (entry[2][0] == 'Z' and '<noBreak>' not in entry[5])):
      SPACE.add(codepoint)
    elif entry[1] != '<control>':
      GRAPH.add(codepoint)

    # upper.
    if entry[13]:
      UPPER.add(codepoint)
      TOLOWER[codepoint] = int(entry[13], 16) - codepoint

# Obtain double-width characters.
with open('EastAsianWidth.txt', 'r') as ufile:
  entries = csv.reader(ufile, delimiter=';')
  for entry in entries:
    if len(entry) >= 2:
      if entry[1][0] in ['F', 'W']:
        codepoints = entry[0].split('..')
        for codepoint in range(int(codepoints[0], 16),
                               int(codepoints[-1], 16) + 1):
          if codepoint in WIDTH and WIDTH[codepoint] == 1:
            WIDTH[codepoint] = 2

# Classes that are simply derived from others.
ALPHA = ALNUM - DIGIT
PUNCT = GRAPH - ALNUM

# Assertions derived from table in POSIX section 7.3.1 LC_CTYPE.
# Automatically included:
assert UPPER.issubset(ALPHA)
assert UPPER.issubset(GRAPH)
assert UPPER.issubset(PRINT)
assert LOWER.issubset(ALPHA)
assert LOWER.issubset(GRAPH)
assert LOWER.issubset(PRINT)
assert ALPHA.issubset(GRAPH)
assert ALPHA.issubset(PRINT)
assert DIGIT.issubset(GRAPH)
assert DIGIT.issubset(PRINT)
assert DIGIT.issubset(XDIGIT)
assert PUNCT.issubset(GRAPH)
assert PUNCT.issubset(PRINT)
assert GRAPH.issubset(PRINT)
assert XDIGIT.issubset(GRAPH)
assert XDIGIT.issubset(PRINT)
assert BLANK.issubset(SPACE)

# Mutually exclusive:
assert not (UPPER & DIGIT)
assert not (UPPER & SPACE)
assert not (UPPER & CNTRL)
assert not (UPPER & PUNCT)
assert not (UPPER & BLANK)
assert not (LOWER & DIGIT)
assert not (LOWER & SPACE)
assert not (LOWER & CNTRL)
assert not (LOWER & PUNCT)
assert not (LOWER & BLANK)
assert not (ALPHA & DIGIT)
assert not (ALPHA & SPACE)
assert not (ALPHA & CNTRL)
assert not (ALPHA & PUNCT)
assert not (ALPHA & BLANK)
assert not (DIGIT & SPACE)
assert not (DIGIT & CNTRL)
assert not (DIGIT & PUNCT)
assert not (DIGIT & BLANK)
assert not (SPACE & XDIGIT)
assert not (CNTRL & PUNCT)
assert not (CNTRL & GRAPH)
assert not (CNTRL & PRINT)
assert not (CNTRL & XDIGIT)
assert not (PUNCT & XDIGIT)
assert not (XDIGIT & BLANK)

# isw*().

def to_type_extents(codepoints):
  extent = None
  for p in sorted(codepoints):
    if extent and p == extent[1] + 1:
      extent = (extent[0], p)
    else:
      if extent:
        yield extent
      extent = (p, p)
  yield extent

def print_type_boolean(extents):
  first = True
  for e in sorted(extents):
    if not first:
      print ' || '
    first = False
    if e[0] == e[1]:
      print 'wc == %#x' % e[0]
    else:
      print '(wc >= %#x && wc <= %#x)' % e

def print_type_table(extents):
  for e in sorted(extents):
    assert e[1] - e[0] < 2048
    print '%#010x,' % (e[0] << 11 | e[1] - e[0])

# tow*().

def to_trans_extents(codepoints):
  extent = None
  for p, d in sorted(codepoints):
    if extent and p == extent[1] + 1 and d == extent[2]:
      extent = (extent[0], p, extent[2])
    else:
      if extent:
        yield extent
      extent = (p, p, d)
  yield extent

def print_delta_table(extents):
  for e in sorted(extents):
    print '%d,' % e[2]

# wcwidth().

def to_width_extents(codepoints):
  extent = None
  for p, w in sorted(codepoints):
    if extent and p == extent[1] + 1 and w == extent[2]:
      extent = (extent[0], p, w)
    else:
      if extent:
        yield extent
      extent = (p, p, w)
  yield extent

def print_width_table(extents):
  for e in sorted(extents):
    while e[1] - e[0] >= 512:
      assert e[2] >= 0 and e[2] < 4
      print '%#010x,' % (e[0] << 11 | e[2] << 9 | 511)
      e = (e[0] + 512, e[1], e[2])
    assert e[2] >= 0 and e[2] < 4
    print '%#010x,' % (e[0] << 11 | e[2] << 9 | e[1] - e[0])

print '--- iswalnum()'
print_type_table(to_type_extents(ALNUM))
print '--- iswblank()'
print_type_boolean(to_type_extents(BLANK))
print '--- iswcntrl()'
print_type_boolean(to_type_extents(CNTRL))
print '--- iswdigit()'
print_type_boolean(to_type_extents(DIGIT))
print '--- iswgraph()'
print_type_table(to_type_extents(GRAPH))
print '--- iswlower()'
print_type_table(to_type_extents(LOWER))
print '--- iswprint()'
print_type_table(to_type_extents(PRINT))
print '--- iswpunct()'
print_type_table(to_type_extents(PUNCT))
print '--- iswspace()'
print_type_boolean(to_type_extents(SPACE))
print '--- iswupper()'
print_type_table(to_type_extents(UPPER))
print '--- iswxdigit()'
print_type_boolean(to_type_extents(XDIGIT))

print '--- towlower()'
t = list(to_trans_extents(TOLOWER.iteritems()))
print_type_table(t)
print '---'
print_delta_table(t)

print '--- towupper()'
t = list(to_trans_extents(TOUPPER.iteritems()))
print_type_table(t)
print '---'
print_delta_table(t)

print '--- wcwidth()'
print_width_table(to_width_extents(WIDTH.iteritems()))
