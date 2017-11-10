// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/uv.h>

#include <limits.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

struct unknown_err {
  struct unknown_err *next;
  int err;
  char message[sizeof("Unknown system error -") + WORD_BIT / 3];
};

const char *__uv_strerror_unknown(int err) {
  // Dynamic error message containing a numerical error code. See if
  // there's already a copy of this error in the stringpool.
  static _Atomic(struct unknown_err *) table = ATOMIC_VAR_INIT(NULL);
  struct unknown_err *first =
      atomic_load_explicit(&table, memory_order_acquire);
  for (struct unknown_err *ue = first; ue != NULL; ue = ue->next)
    if (ue->err == err)
      return ue->message;

  // Allocate a copy to be stored in the stringpool.
  struct unknown_err *ue_new = malloc(sizeof(*ue_new));
  if (ue_new == NULL)
    return "Unknown system error";
  ue_new->err = err;
  snprintf(ue_new->message, sizeof(ue_new->message), "Unknown system error %d",
           err);

  for (;;) {
    // Store the new entry in the global list. The list is lockless, so
    // perform a compare-and-exchange.
    ue_new->next = first;
    if (atomic_compare_exchange_weak(&table, &first, ue_new))
      return ue_new->message;

    // Compare-and-exchange failed. See if an entry for the same
    // abbreviation got created in the meantime.
    for (struct unknown_err *ue = first; ue != ue_new->next; ue = ue->next)
      if (ue->err == err) {
        free(ue_new);
        return ue->message;
      }
  }
}
