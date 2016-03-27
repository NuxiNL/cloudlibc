// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>

#include <argdata.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

bool argdata_has_next(argdata_iterator_t *it_) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  return it->left != 0;
}

int argdata_map_iterate(const argdata_t *ad, argdata_iterator_t *it_) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  it->container = ad;
  it->index = (size_t)-1;
  it->error = 0;
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      it->left = ad->length;
      it->error = parse_type(ADT_MAP, &buf, &it->left);
      break;
    }
    case AD_MAP:
      it->left = ad->map.count;
      break;
    default:
      it->error = EINVAL;
  }
  return it->error;
}

int argdata_seq_iterate(const argdata_t *ad, argdata_iterator_t *it_) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  it->container = ad;
  it->index = (size_t)-1;
  it->error = 0;
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      it->left = ad->length;
      it->error = parse_type(ADT_SEQ, &buf, &it->left);
      break;
    }
    case AD_SEQ:
      it->left = ad->seq.count;
      break;
    default:
      it->error = EINVAL;
  }
  return it->error;
}

bool argdata_map_next(argdata_iterator_t *it_,
                      const argdata_t **key,
                      const argdata_t **value) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  const argdata_t *ad = it->container;

  // Iterating already finished.
  if (ad == NULL)
    it->error = EINVAL;

  // Iterator has error.
  if (it->error != 0)
    return false;

  // Reached end.
  if (it->left == 0) {
    ++it->index;
    it->container = NULL;
    return false;
  }

  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer + ad->length - it->left;
      int error = parse_subfield(&it->key, &buf, &it->left);
      if (error != 0) {
        it->error = error;
        return false;
      }
      error = parse_subfield(&it->value, &buf, &it->left);
      if (error != 0) {
        it->error = error;
        return false;
      }
      ++it->index;
      *key = &it->key;
      *value = &it->value;
      return true;
    }
    case AD_MAP: {
      it->index = ad->map.count - it->left;
      *key = ad->map.keys[it->index];
      *value = ad->map.values[it->index];
      --it->left;
      return true;
    }
    default:
      it->error = EINVAL;
      return false;
  }
}

bool argdata_seq_next(argdata_iterator_t *it_,
                      const argdata_t **value) {
  struct __argdata_iterator *it = (struct __argdata_iterator*)it_;
  const argdata_t *ad = it->container;

  // Iterating already finished.
  if (ad == NULL)
    it->error = EINVAL;

  // Iterator has error.
  if (it->error != 0)
    return false;

  // Reached end.
  if (it->left == 0) {
    ++it->index;
    it->container = NULL;
    return false;
  }

  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer + ad->length - it->left;
      it->error = parse_subfield(&it->value, &buf, &it->left);
      if (it->error != 0)
        return false;
      ++it->index;
      *value = &it->value;
      return true;
    }
    case AD_SEQ: {
      it->index = ad->seq.count - it->left;
      *value = ad->seq.entries[it->index];
      --it->left;
      return true;
    }
    default:
      it->error = EINVAL;
      return false;
  }
}
