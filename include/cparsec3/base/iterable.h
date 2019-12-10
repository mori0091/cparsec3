/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#include "itr.h"

#define Iterable(C) TYPE_NAME(Iterable, C)

// -----------------------------------------------------------------------
#define trait_Iterable(C)                                                \
  C_API_BEGIN                                                            \
  /* ---- trait Iterable(C) */                                           \
  typedef struct Iterable(C) Iterable(C);                                \
  struct Iterable(C) {                                                   \
    Itr(C) (*itr)(C c);                                                  \
  };                                                                     \
  Iterable(C) Trait(Iterable(C));                                        \
  /* ---- trait Iterable(Itr(C)) ... i.e. Itr(C) is also iterable */     \
  typedef struct Iterable(Itr(C)) Iterable(Itr(C));                      \
  struct Iterable(Itr(C)) {                                              \
    Itr(C) (*itr)(Itr(C) it);                                            \
  };                                                                     \
  Iterable(Itr(C)) Trait(Iterable(Itr(C)));                              \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Iterable(C, _itr_)                                      \
  C_API_BEGIN                                                            \
  /* ---- trait Iterable(C) */                                           \
  Iterable(C) Trait(Iterable(C)) {                                       \
    return (Iterable(C)){                                                \
        .itr = _itr_,                                                    \
    };                                                                   \
  }                                                                      \
  /* ---- trait Iterable(Itr(C)) */                                      \
  static Itr(C) FUNC_NAME(itr, Iterable(Itr(C)))(Itr(C) it) {            \
    return it;                                                           \
  }                                                                      \
  Iterable(Itr(C)) Trait(Iterable(Itr(C))) {                             \
    return (Iterable(Itr(C))){                                           \
        .itr = FUNC_NAME(itr, Iterable(Itr(C))),                         \
    };                                                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
