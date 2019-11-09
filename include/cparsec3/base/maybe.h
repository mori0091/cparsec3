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
    Maybe(T) (*just)(T value);                                           \
    union {                                                              \
      Maybe(T) (*none)(void);                                            \
      Maybe(T) (*nothing)(void);                                         \
    };                                                                   \
  } MaybeT(T);                                                           \
  MaybeT(T) Trait(Maybe(T));                                             \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Maybe(T)                                                    \
  C_API_BEGIN                                                            \
  impl_Eq_Maybe(T);                                                      \
  impl_Ord_Maybe(T);                                                     \
  static Maybe(T) FUNC_NAME(just, Maybe(T))(T value) {                   \
    return (Maybe(T)){.value = value};                                   \
  }                                                                      \
  static Maybe(T) FUNC_NAME(none, Maybe(T))(void) {                      \
    return (Maybe(T)){.none = true};                                     \
  }                                                                      \
  MaybeT(T) Trait(Maybe(T)) {                                            \
    return (MaybeT(T)){                                                  \
        .just = FUNC_NAME(just, Maybe(T)),                               \
        .none = FUNC_NAME(none, Maybe(T)),                               \
    };                                                                   \
  }                                                                      \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Eq_Maybe(T)                                                 \
  static bool FUNC_NAME(eq, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    if (a.none && b.none) {                                              \
      return true;                                                       \
    }                                                                    \
    if (a.none != b.none) {                                              \
      return false;                                                      \
    }                                                                    \
    return trait(Eq(T)).eq(a.value, b.value);                            \
  }                                                                      \
  static bool FUNC_NAME(neq, Maybe(T))(Maybe(T) a, Maybe(T) b) {         \
    return !FUNC_NAME(eq, Maybe(T))(a, b);                               \
  }                                                                      \
  Eq(Maybe(T)) Trait(Eq(Maybe(T))) {                                     \
    return (Eq(Maybe(T))){                                               \
        .eq = FUNC_NAME(eq, Maybe(T)),                                   \
        .neq = FUNC_NAME(neq, Maybe(T)),                                 \
    };                                                                   \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Ord_Maybe(T)                                                \
  static bool FUNC_NAME(le, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    if (a.none) {                                                        \
      return true;                                                       \
    }                                                                    \
    if (b.none) {                                                        \
      return false;                                                      \
    }                                                                    \
    return trait(Ord(T)).le(a.value, b.value);                           \
  }                                                                      \
  static bool FUNC_NAME(lt, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    return FUNC_NAME(le, Maybe(T))(a, b) &&                              \
           !FUNC_NAME(eq, Maybe(T))(a, b);                               \
  }                                                                      \
  static bool FUNC_NAME(ge, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    return FUNC_NAME(le, Maybe(T))(b, a);                                \
  }                                                                      \
  static bool FUNC_NAME(gt, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    return FUNC_NAME(lt, Maybe(T))(b, a);                                \
  }                                                                      \
  static int FUNC_NAME(cmp, Maybe(T))(Maybe(T) a, Maybe(T) b) {          \
    return (FUNC_NAME(le, Maybe(T))(a, b)                                \
                ? (FUNC_NAME(eq, Maybe(T))(a, b) ? 0 : -1)               \
                : 1);                                                    \
  }                                                                      \
  static Maybe(T) FUNC_NAME(min, Maybe(T))(Maybe(T) a, Maybe(T) b) {     \
    return FUNC_NAME(le, Maybe(T))(a, b) ? a : b;                        \
  }                                                                      \
  static Maybe(T) FUNC_NAME(max, Maybe(T))(Maybe(T) a, Maybe(T) b) {     \
    return FUNC_NAME(le, Maybe(T))(b, a) ? a : b;                        \
  }                                                                      \
  Ord(Maybe(T)) Trait(Ord(Maybe(T))) {                                   \
    return (Ord(Maybe(T))){                                              \
        .cmp = FUNC_NAME(cmp, Maybe(T)),                                 \
        .lt = FUNC_NAME(lt, Maybe(T)),                                   \
        .le = FUNC_NAME(le, Maybe(T)),                                   \
        .gt = FUNC_NAME(gt, Maybe(T)),                                   \
        .ge = FUNC_NAME(ge, Maybe(T)),                                   \
        .min = FUNC_NAME(min, Maybe(T)),                                 \
        .max = FUNC_NAME(max, Maybe(T)),                                 \
    };                                                                   \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
FOREACH(trait_Maybe, TYPESET(ALL));
