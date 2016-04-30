// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <libgen.h>
#include <stdbool.h>
#include <string.h>

char *dirname(char *path) {
  // If path is a null pointer or points to an empty string, dirname()
  // shall return a pointer to the string ".".
  if (path == NULL || *path == '\0')
    return (char *)".";

  // Retain at least one leading slash character.
  char *out = *path == '/' ? path + 1 : path;
  const char *in = out;

  bool skipslash = true;
  const char *prev = ".";
  size_t prevlen = 1;
  while (*in != '\0') {
    // Extract the next pathname component.
    const char *begin = in = in + strspn(in, "/");
    const char *end = in = in + strcspn(in, "/");
    if (begin == end)
      break;

    // Copy over the previous pathname component, except if it's dot.
    // There is no point in retaining those.
    if (prevlen != 1 || *prev != '.') {
      if (!skipslash)
        *out++ = '/';
      skipslash = false;
      memmove(out, prev, prevlen);
      out += prevlen;
    }

    // Preserve the pathname component for the next iteration.
    prev = begin;
    prevlen = end - begin;
  }

  // If path does not contain a '/', then dirname() shall return a
  // pointer to the string ".".
  if (out == path)
    *out++ = '.';
  *out = '\0';
  return path;
}
