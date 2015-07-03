// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

static void subfield(const argdata_t *, uint8_t **);

static void encode(const argdata_t *ad, uint8_t *buf) {
  // Skip empty nodes.
  if (ad->length == 0)
    return;

  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *ibuf = ad->buffer;
      size_t ilen = ad->length - 1;

      // Copy over the field type byte.
      switch ((*buf++ = *ibuf++)) {
        case ADT_MAP:
        case ADT_SEQ: {
          // Scan map and sequence entries for file descriptors.
          for (;;) {
            argdata_t iad;
            const uint8_t *lenstart = ibuf;
            if (parse_subfield(&iad, &ibuf, &ilen) != 0)
              break;
            // Copy over the length field.
            size_t lenlen = iad.buffer - lenstart;
            memcpy(buf, lenstart, lenlen);
            buf += lenlen;
            // Process the body of the entry.
            encode(&iad, buf);
            buf += iad.length;
          }
          break;
        }
        case ADT_FD:
          if (ilen != sizeof(uint32_t))
            break;

          // TODO(edje): Translate the file descriptor number.
          break;
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
        subfield(ad->map.keys[i], &buf);
        subfield(ad->map.values[i], &buf);
      }
      break;
    case AD_SEQ:
      // Emit every sequence entry.
      *buf++ = ADT_SEQ;
      for (size_t i = 0; i < ad->map.count; ++i)
        subfield(ad->seq.entries[i], &buf);
      break;
    case AD_STR:
      // Copy over the string payload and add a terminating null byte.
      *buf++ = ADT_STR;
      memcpy(buf, ad->str, ad->length - 2);
      buf[ad->length - 2] = '\0';
      break;
  }
}

static void subfield(const argdata_t *ad, uint8_t **buf) {
  put_subfield_length(ad, buf);
  encode(ad, *buf);
  buf += ad->length;
}

int __argdata_generate(const argdata_t *ad, void *buf, int **fds,
                       size_t *fdslen) {
  encode(ad, buf);
  // TODO(edje): Return list of file descriptors.
  *fds = NULL;
  *fdslen = 0;
  return ENOSYS;
}
