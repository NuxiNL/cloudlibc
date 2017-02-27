// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>
#include <string.h>

#include "argdata_impl.h"

static void encode_subfield(const argdata_t *, uint8_t **, int *, size_t *);

static int map_fd(int fd, int *fds, size_t *fdslen) {
  for (size_t i = 0; i < *fdslen; ++i)
    if (fds[i] == fd)
      return i;
  size_t newfd = (*fdslen)++;
  fds[newfd] = fd;
  return newfd;
}

static void encode(const argdata_t *ad, uint8_t *buf, int *fds,
                   size_t *fdslen) {
  // Skip empty nodes.
  if (ad->length == 0)
    return;

  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *ibuf = ad->buffer;
      size_t ilen = ad->length;

      if (fds != NULL) {
        // Copy over the field type byte.
        --ilen;
        switch ((*buf++ = *ibuf++)) {
          case ADT_MAP:
          case ADT_SEQ: {
            // Scan map and sequence entries for file descriptors.
            for (;;) {
              argdata_t iad;
              if (parse_subfield(&iad, &ibuf, &ilen) != 0)
                break;
              encode_subfield(&iad, &buf, fds, fdslen);
            }
            break;
          }
          case ADT_FD: {
            // Remap file descriptors to be sequential starting at zero.
            int fd;
            if (parse_fd(&fd, &ibuf, &ilen) == 0)
              encode_fd(map_fd(fd, fds, fdslen), &buf);
            break;
          }
        }
      }

      // (Remainder of the) payload that is unsupported by this
      // implementation or does not contain a file descriptor. Copy it
      // over literally, so that its structure remains identical to the
      // original.
      memcpy(buf, ibuf, ilen);
      break;
    }
    case AD_BINARY:
      // Copy over the binary payload.
      *buf++ = ADT_BINARY;
      memcpy(buf, ad->binary, ad->length - 1);
      break;
    case AD_MAP:
      // Emit key and value for every map entry.
      *buf++ = ADT_MAP;
      for (size_t i = 0; i < ad->map.count; ++i) {
        encode_subfield(ad->map.keys[i], &buf, fds, fdslen);
        encode_subfield(ad->map.values[i], &buf, fds, fdslen);
      }
      break;
    case AD_SEQ:
      // Emit every sequence entry.
      *buf++ = ADT_SEQ;
      for (size_t i = 0; i < ad->seq.count; ++i)
        encode_subfield(ad->seq.entries[i], &buf, fds, fdslen);
      break;
    case AD_STR:
      // Copy over the string payload and add a terminating null byte.
      *buf++ = ADT_STR;
      memcpy(buf, ad->str, ad->length - 2);
      buf[ad->length - 2] = '\0';
      break;
  }
}

static void encode_subfield(const argdata_t *ad, uint8_t **buf, int *fds,
                            size_t *fdslen) {
  encode_subfield_length(ad, buf);
  encode(ad, *buf, fds, fdslen);
  *buf += ad->length;
}

size_t argdata_serialize(const argdata_t *ad, void *buf, int *fds) {
  size_t nfds = 0;
  encode(ad, buf, fds, &nfds);
  return nfds;
}
