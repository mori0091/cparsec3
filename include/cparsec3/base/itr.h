/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#define Item(C) TYPE_NAME(Item, C)
#define Itr(C) TYPE_NAME(Itr, C)
#define ItrT(C) TYPE_NAME(ItrT, C)

// -----------------------------------------------------------------------
#define trait_Itr(C)                                                     \
  C_API_BEGIN                                                            \
  /* ---- trait Itr(C) */                                                \
  typedef struct ItrT(C) ItrT(C);                                        \
  struct ItrT(C) {                                                       \
    Item(C) * (*ptr)(Itr(C) it);                                         \
    Itr(C) (*next)(Itr(C) it);                                           \
    Itr(C) (*skip)(size_t n, Itr(C) it);                                 \
    bool (*null)(Itr(C) it);                                             \
    Item(C) (*get)(Itr(C) it);                                           \
    void (*set)(Item(C) x, Itr(C) it);                                   \
  };                                                                     \
  ItrT(C) Trait(Itr(C));                                                 \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Itr(C, _ptr_, _next_)                                   \
  C_API_BEGIN                                                            \
  /* ---- trait Itr(C) */                                                \
  static Itr(C) FUNC_NAME(skip, Itr(C))(size_t n, Itr(C) it) {           \
    while (_ptr_(it) && n) {                                             \
      it = _next_(it);                                                   \
      n--;                                                               \
    }                                                                    \
    return it;                                                           \
  }                                                                      \
  static bool FUNC_NAME(null, Itr(C))(Itr(C) it) {                       \
    return !_ptr_(it);                                                   \
  }                                                                      \
  static Item(C) FUNC_NAME(get, Itr(C))(Itr(C) it) {                     \
    Item(C)* p = _ptr_(it);                                              \
    assert(p);                                                           \
    return *p;                                                           \
  }                                                                      \
  static void FUNC_NAME(set, Itr(C))(Item(C) v, Itr(C) it) {             \
    Item(C)* p = _ptr_(it);                                              \
    *p = v;                                                              \
  }                                                                      \
  ItrT(C) Trait(Itr(C)) {                                                \
    return (ItrT(C)){                                                    \
        .ptr = _ptr_,                                                    \
        .next = _next_,                                                  \
        .skip = FUNC_NAME(skip, Itr(C)),                                 \
        .null = FUNC_NAME(null, Itr(C)),                                 \
        .get = FUNC_NAME(get, Itr(C)),                                   \
        .set = FUNC_NAME(set, Itr(C)),                                   \
    };                                                                   \
  }                                                                      \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS
