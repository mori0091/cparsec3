/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#include "eq.h"
#include "ord.h"

#define Maybe(T) TYPE_NAME(Maybe, T)
#define MaybeT(T) TYPE_NAME(MaybeT, T)
// -----------------------------------------------------------------------
#define typedef_Maybe(T)                                                 \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    bool none;                                                           \
    T value;                                                             \
  } Maybe(T);                                                            \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define trait_Maybe(T)                                                   \
  C_API_BEGIN                                                            \
  typedef_Maybe(T);                                                      \
  trait_Eq(Maybe(T));                                                    \
  trait_Ord(Maybe(T));                                                   \
  typedef struct {                                                       \
    Maybe(T) empty;                                                      \
    bool (*null)(Maybe(T) m);                                            \
    size_t (*length)(Maybe(T) m);                                        \
    Maybe(T) (*just)(T value);                                           \
  } MaybeT(T);                                                           \
  MaybeT(T) Trait(Maybe(T));                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Maybe(T)                                                    \
  C_API_BEGIN                                                            \
  /* ---- trait Maybe(T) */                                              \
  static bool FUNC_NAME(null, Maybe(T))(Maybe(T) m) {                    \
    return m.none;                                                       \
  }                                                                      \
  static size_t FUNC_NAME(length, Maybe(T))(Maybe(T) m) {                \
    return m.none ? 0 : 1;                                               \
  }                                                                      \
  static Maybe(T) FUNC_NAME(just, Maybe(T))(T value) {                   \
    return (Maybe(T)){.value = value};                                   \
  }                                                                      \
  MaybeT(T) Trait(Maybe(T)) {                                            \
    return (MaybeT(T)){                                                  \
        .empty = {0},                                                    \
        .null = FUNC_NAME(null, Maybe(T)),                               \
        .length = FUNC_NAME(length, Maybe(T)),                           \
        .just = FUNC_NAME(just, Maybe(T)),                               \
    };                                                                   \
  }                                                                      \
  /* ---- instance Eq(Maybe(T)) */                                       \
  static bool FUNC_NAME(eq, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    if (a.none && b.none) {                                              \
      return true;                                                       \
    }                                                                    \
    if (a.none != b.none) {                                              \
      return false;                                                      \
    }                                                                    \
    return trait(Eq(T)).eq(a.value, b.value);                            \
  }                                                                      \
  instance_Eq(Maybe(T), FUNC_NAME(eq, Maybe(T)));                        \
  /* ---- instance Ord(Maybe(T)) */                                      \
  static bool FUNC_NAME(le, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    if (a.none) {                                                        \
      return true;                                                       \
    }                                                                    \
    if (b.none) {                                                        \
      return false;                                                      \
    }                                                                    \
    return trait(Ord(T)).le(a.value, b.value);                           \
  }                                                                      \
  instance_Ord(Maybe(T), FUNC_NAME(le, Maybe(T)),                        \
               FUNC_NAME(eq, Maybe(T)));                                 \
  /* ---- */                                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
FOREACH(trait_Maybe, TYPESET(ALL));
