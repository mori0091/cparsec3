/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

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
  /* ---- Slice(C) */                                                    \
  typedef struct {                                                       \
    size_t start;                                                        \
    size_t stop;                                                         \
    C c;                                                                 \
  } Slice(C);                                                            \
  /* ---- trait Slice(C) */                                              \
  typedef struct {                                                       \
    Slice(C) empty;                                                      \
    bool (*null)(Slice(C) s);                                            \
    size_t (*length)(Slice(C) s);                                        \
    Slice(C) (*slice)(C c, int start, int stop);                         \
  } SliceT(C);                                                           \
  SliceT(C) Trait(Slice(C));                                             \
  /* ---- instance Itr(Slice(C)) */                                      \
  typedef Item(C) Item(Slice(C));                                        \
  typedef struct {                                                       \
    size_t rest;                                                         \
    Itr(C) it;                                                           \
  } Itr(Slice(C));                                                       \
  trait_Itr(Slice(C));                                                   \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Slice(C)                                                \
  C_API_BEGIN                                                            \
  /* ---- trait Slice(C) */                                              \
  static bool FUNC_NAME(null, Slice(C))(Slice(C) s) {                    \
    assert(s.start <= s.stop);                                           \
    return (s.start == s.stop);                                          \
  }                                                                      \
  static size_t FUNC_NAME(length, Slice(C))(Slice(C) s) {                \
    assert(s.start <= s.stop);                                           \
    return (s.stop - s.start);                                           \
  }                                                                      \
  static Slice(C) FUNC_NAME(slice, Slice(C))(C c, int start, int stop) { \
    if (trait(C).null(c)) {                                              \
      return (Slice(C)){0};                                              \
    }                                                                    \
    size_t len = trait(C).length(c);                                     \
    size_t s1 = adjust_index(start, len);                                \
    size_t s2 = adjust_index(stop, len);                                 \
    if (s1 >= s2) {                                                      \
      return (Slice(C)){0};                                              \
    }                                                                    \
    return (Slice(C)){.start = s1, .stop = s2, .c = c};                  \
  }                                                                      \
  SliceT(C) Trait(Slice(C)) {                                            \
    return (SliceT(C)){                                                  \
        .empty = {0},                                                    \
        .null = FUNC_NAME(null, Slice(C)),                               \
        .length = FUNC_NAME(length, Slice(C)),                           \
        .slice = FUNC_NAME(slice, Slice(C)),                             \
    };                                                                   \
  }                                                                      \
  /* ---- instance Itr(Slice(C))*/                                       \
  static Itr(Slice(C)) FUNC_NAME(itr, Itr(Slice(C)))(Slice(C) s) {       \
    ItrT(C) I = trait(Itr(C));                                           \
    Itr(C) it = I.skip(s.start, I.itr(s.c));                             \
    return (Itr(Slice(C))){                                              \
        .rest = FUNC_NAME(length, Slice(C))(s),                          \
        .it = it,                                                        \
    };                                                                   \
  }                                                                      \
  static Item(C) * FUNC_NAME(ptr, Itr(Slice(C)))(Itr(Slice(C)) it) {     \
    return (it.rest ? trait(Itr(C)).ptr(it.it) : 0);                     \
  }                                                                      \
  static Itr(Slice(C))                                                   \
      FUNC_NAME(next, Itr(Slice(C)))(Itr(Slice(C)) it) {                 \
    ItrT(C) I = trait(Itr(C));                                           \
    assert(it.rest && !I.null(it.it));                                   \
    it.rest--;                                                           \
    it.it = I.next(it.it);                                               \
    return it;                                                           \
  }                                                                      \
  instance_Itr(Slice(C), FUNC_NAME(itr, Itr(Slice(C))),                  \
               FUNC_NAME(ptr, Itr(Slice(C))),                            \
               FUNC_NAME(next, Itr(Slice(C))));                          \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
