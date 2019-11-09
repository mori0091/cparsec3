/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#define Eq(T) TYPE_NAME(Eq, T)
// -----------------------------------------------------------------------
#define trait_Eq(T)                                                      \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    bool (*eq)(T, T);                                                    \
    bool (*neq)(T, T);                                                   \
  } Eq(T);                                                               \
  Eq(T) Trait(Eq(T));                                                    \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Eq(T, _eq_)                                             \
  C_API_BEGIN                                                            \
  static bool FUNC_NAME(neq, Eq(T))(T a, T b) {                          \
    return !_eq_(a, b);                                                  \
  }                                                                      \
  Eq(T) Trait(Eq(T)) {                                                   \
    return (Eq(T)){                                                      \
        .eq = _eq_,                                                      \
        .neq = FUNC_NAME(neq, Eq(T)),                                    \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
FOREACH(trait_Eq, TYPESET(ALL));
