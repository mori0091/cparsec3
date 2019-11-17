/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#define Item(C) TYPE_NAME(Item, C)
#define Itr(C) TYPE_NAME(Itr, C)
#define ItrT(C) TYPE_NAME(ItrT, C)

// -----------------------------------------------------------------------
#define trait_Itr(C)                                                     \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    Itr(C) (*itr)(C c);                                                  \
    Item(C) * (*ptr)(Itr(C) it);                                         \
    Itr(C) (*next)(Itr(C) it);                                           \
    bool (*null)(Itr(C) it);                                             \
    Item(C) (*get)(Itr(C) it);                                           \
    void (*set)(Item(C) x, Itr(C) it);                                   \
  } ItrT(C);                                                             \
  ItrT(C) Trait(Itr(C));                                                 \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Itr(C, _itr_, _ptr_, _next_)                            \
  C_API_BEGIN                                                            \
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
        .itr = _itr_,                                                    \
        .ptr = _ptr_,                                                    \
        .next = _next_,                                                  \
        .null = FUNC_NAME(null, Itr(C)),                                 \
        .get = FUNC_NAME(get, Itr(C)),                                   \
        .set = FUNC_NAME(set, Itr(C)),                                   \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
