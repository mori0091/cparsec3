/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "itr.h"

static inline size_t adjust_index(int idx, size_t len) {
  if (idx < 0) {
    idx += len;
  }
  return (idx < 0 ? 0 : ((size_t)idx <= len ? (size_t)idx : len));
}

#define Slice(C) TYPE_NAME(Slice, C)
#define SliceT(C) TYPE_NAME(SliceT, C)

// -----------------------------------------------------------------------
#define trait_Slice(C)                                                   \
  C_API_BEGIN                                                            \
  /* ---- trait Slice(C) */                                              \
  typedef struct SliceT(C) SliceT(C);                                    \
  struct SliceT(C) {                                                     \
    Slice(C) empty;                                                      \
    bool (*null)(Slice(C) s);                                            \
    size_t (*length)(Slice(C) s);                                        \
    void (*reverse)(Slice(C) * ps);                                      \
    Slice(C) (*slice)(C c, int start, int stop);                         \
  };                                                                     \
  SliceT(C) Trait(Slice(C));                                             \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
