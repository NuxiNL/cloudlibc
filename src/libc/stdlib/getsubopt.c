// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <string.h>

int getsubopt(char **optionp, char *const *keylistp, char **valuep) {
  // Obtain key and value of first entry in the string.
  size_t end = strcspn(*optionp, ",");
  char *equals = memchr(*optionp, '=', end);
  size_t keylen = equals != NULL ? (size_t)(equals - *optionp) : end;
  if (keylen == 0)
    return -1;

  // Look up the index corresponding to this key.
  int key = 0;
  for (;;) {
    if (keylistp[key] == NULL)
      return -1;
    if (memcmp(keylistp[key], *optionp, keylen) == 0 &&
        keylistp[key][keylen] == '\0')
      break;
    ++key;
  }

  // Progress to the next key and return the value of this key.
  *optionp += end;
  if (**optionp != '\0')
    *(*optionp)++ = '\0';
  *valuep = equals != NULL ? equals + 1 : NULL;
  return key;
}
