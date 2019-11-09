/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"
#include "typeset.h"

#define Ord(T) TYPE_NAME(Ord, T)
// -----------------------------------------------------------------------
#define trait_Ord(T)                                                     \
  C_API_BEGIN                                                            \
  typedef struct {                                                       \
    union {                                                              \
      int (*cmp)(T, T);                                                  \
      int (*compare)(T, T);                                              \
    };                                                                   \
    union {                                                              \
      bool (*lt)(T, T);                                                  \
      bool (*lessThan)(T, T);                                            \
    };                                                                   \
    union {                                                              \
      bool (*le)(T, T);                                                  \
      bool (*lessThanEqual)(T, T);                                       \
    };                                                                   \
    union {                                                              \
      bool (*gt)(T, T);                                                  \
      bool (*greaterThan)(T, T);                                         \
    };                                                                   \
    union {                                                              \
      bool (*ge)(T, T);                                                  \
      bool (*greaterThanEqual)(T, T);                                    \
    };                                                                   \
    T (*min)(T, T);                                                      \
    T (*max)(T, T);                                                      \
  } Ord(T);                                                              \
  Ord(T) Trait(Ord(T));                                                  \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Ord(T, ...)                                             \
  CAT(instance_Ord_, VARIADIC_SIZE(__VA_ARGS__))(T, __VA_ARGS__)

#define instance_Ord_1 instance_Ord_cmp
#define instance_Ord_2 instance_Ord_le_eq

// -----------------------------------------------------------------------
#define instance_Ord_cmp(T, _cmp_)                                       \
  static bool FUNC_NAME(lt, T)(T a, T b) {                               \
    return _cmp_(a, b) < 0;                                              \
  }                                                                      \
  static bool FUNC_NAME(le, T)(T a, T b) {                               \
    return _cmp_(a, b) <= 0;                                             \
  }                                                                      \
  static bool FUNC_NAME(gt, T)(T a, T b) {                               \
    return _cmp_(a, b) > 0;                                              \
  }                                                                      \
  static bool FUNC_NAME(ge, T)(T a, T b) {                               \
    return _cmp_(a, b) >= 0;                                             \
  }                                                                      \
  static T FUNC_NAME(min, T)(T a, T b) {                                 \
    return _cmp_(a, b) <= 0 ? a : b;                                     \
  }                                                                      \
  static T FUNC_NAME(max, T)(T a, T b) {                                 \
    return _cmp_(a, b) >= 0 ? a : b;                                     \
  }                                                                      \
  Ord(T) Trait(Ord(T)) {                                                 \
    return (Ord(T)){.cmp = _cmp_,                                        \
                    .lt = FUNC_NAME(lt, T),                              \
                    .le = FUNC_NAME(le, T),                              \
                    .gt = FUNC_NAME(gt, T),                              \
                    .ge = FUNC_NAME(ge, T),                              \
                    .min = FUNC_NAME(min, T),                            \
                    .max = FUNC_NAME(max, T)};                           \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Ord_le_eq(T, _le_, _eq_)                                \
  static int FUNC_NAME(cmp, T)(T a, T b) {                               \
    return (_le_(a, b) ? (_eq_(a, b) ? 0 : -1) : 1);                     \
  }                                                                      \
  static bool FUNC_NAME(lt, T)(T a, T b) {                               \
    return _le_(a, b) && !_eq_(a, b);                                    \
  }                                                                      \
  static bool FUNC_NAME(ge, T)(T a, T b) {                               \
    return _le_(b, a);                                                   \
  }                                                                      \
  static bool FUNC_NAME(gt, T)(T a, T b) {                               \
    return FUNC_NAME(lt, T)(b, a);                                       \
  }                                                                      \
  static T FUNC_NAME(min, T)(T a, T b) {                                 \
    return _le_(a, b) ? a : b;                                           \
  }                                                                      \
  static T FUNC_NAME(max, T)(T a, T b) {                                 \
    return _le_(b, a) ? a : b;                                           \
  }                                                                      \
  Ord(T) Trait(Ord(T)) {                                                 \
    return (Ord(T)){                                                     \
        .cmp = FUNC_NAME(cmp, T),                                        \
        .lt = FUNC_NAME(lt, T),                                          \
        .le = _le_,                                                      \
        .gt = FUNC_NAME(gt, T),                                          \
        .ge = FUNC_NAME(ge, T),                                          \
        .min = FUNC_NAME(min, T),                                        \
        .max = FUNC_NAME(max, T),                                        \
    };                                                                   \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
FOREACH(trait_Ord, TYPESET(ALL));
