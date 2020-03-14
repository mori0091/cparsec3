/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#define Eq(T) TYPE_NAME(Eq, T)
// -----------------------------------------------------------------------
#define trait_Eq(T)                                                      \
  C_API_BEGIN                                                            \
  typedef struct Eq(T) Eq(T);                                            \
  struct Eq(T) {                                                         \
    bool (*eq)(T, T);                                                    \
    bool (*neq)(T, T);                                                   \
  };                                                                     \
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
#define impl_Eq_Maybe(T)                                                 \
  C_API_BEGIN                                                            \
  static bool FUNC_NAME(eq, Eq(Maybe(T)))(Maybe(T) a, Maybe(T) b) {      \
    if (a.none && b.none) {                                              \
      return true;                                                       \
    }                                                                    \
    if (a.none != b.none) {                                              \
      return false;                                                      \
    }                                                                    \
    return trait(Eq(T)).eq(a.value, b.value);                            \
  }                                                                      \
  instance_Eq(Maybe(T), FUNC_NAME(eq, Eq(Maybe(T))));                    \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Eq_Array(T)                                                 \
  C_API_BEGIN                                                            \
  static bool FUNC_NAME(eq, Eq(Array(T)))(Array(T) a, Array(T) b) {      \
    if (a.length != b.length) {                                          \
      return false;                                                      \
    }                                                                    \
    if (a.data == b.data) {                                              \
      return true;                                                       \
    }                                                                    \
    for (size_t i = 0; i < a.length; ++i) {                              \
      if (trait(Eq(T)).neq(a.data[i], b.data[i])) {                      \
        return false;                                                    \
      }                                                                  \
    }                                                                    \
    return true;                                                         \
  }                                                                      \
  instance_Eq(Array(T), FUNC_NAME(eq, Eq(Array(T))));                    \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Eq_List(T)                                                  \
  C_API_BEGIN                                                            \
  static bool FUNC_NAME(eq, Eq(List(T)))(List(T) a, List(T) b) {         \
    for (;;) {                                                           \
      if (a == b) {                                                      \
        return true;                                                     \
      }                                                                  \
      if (!a || !b) {                                                    \
        return false;                                                    \
      }                                                                  \
      if (trait(Eq(T)).neq(a->head, b->head)) {                          \
        return false;                                                    \
      }                                                                  \
      a = a->tail;                                                       \
      b = b->tail;                                                       \
    }                                                                    \
  }                                                                      \
  instance_Eq(List(T), FUNC_NAME(eq, Eq(List(T))));                      \
  C_API_END                                                              \
  END_OF_STATEMENTS
