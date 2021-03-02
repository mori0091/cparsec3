/* -*- coding: utf-8-unix -*- */
#pragma once

#include "common.h"
#include "trait.h"

#define Ord(T) TYPE_NAME(Ord, T)
// -----------------------------------------------------------------------
#define trait_Ord(T)                                                     \
  C_API_BEGIN                                                            \
  typedef struct Ord(T) Ord(T);                                          \
  struct Ord(T) {                                                        \
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
  };                                                                     \
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
  static bool FUNC_NAME(lt, Ord(T))(T a, T b) {                          \
    return _cmp_(a, b) < 0;                                              \
  }                                                                      \
  static bool FUNC_NAME(le, Ord(T))(T a, T b) {                          \
    return _cmp_(a, b) <= 0;                                             \
  }                                                                      \
  static bool FUNC_NAME(gt, Ord(T))(T a, T b) {                          \
    return _cmp_(a, b) > 0;                                              \
  }                                                                      \
  static bool FUNC_NAME(ge, Ord(T))(T a, T b) {                          \
    return _cmp_(a, b) >= 0;                                             \
  }                                                                      \
  static T FUNC_NAME(min, Ord(T))(T a, T b) {                            \
    return _cmp_(a, b) <= 0 ? a : b;                                     \
  }                                                                      \
  static T FUNC_NAME(max, Ord(T))(T a, T b) {                            \
    return _cmp_(a, b) >= 0 ? a : b;                                     \
  }                                                                      \
  Ord(T) Trait(Ord(T)) {                                                 \
    return (Ord(T)){.cmp = _cmp_,                                        \
                    .lt = FUNC_NAME(lt, Ord(T)),                         \
                    .le = FUNC_NAME(le, Ord(T)),                         \
                    .gt = FUNC_NAME(gt, Ord(T)),                         \
                    .ge = FUNC_NAME(ge, Ord(T)),                         \
                    .min = FUNC_NAME(min, Ord(T)),                       \
                    .max = FUNC_NAME(max, Ord(T))};                      \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define instance_Ord_le_eq(T, _le_, _eq_)                                \
  static int FUNC_NAME(cmp, Ord(T))(T a, T b) {                          \
    return (_le_(a, b) ? (_eq_(a, b) ? 0 : -1) : 1);                     \
  }                                                                      \
  static bool FUNC_NAME(lt, Ord(T))(T a, T b) {                          \
    return _le_(a, b) && !_eq_(a, b);                                    \
  }                                                                      \
  static bool FUNC_NAME(ge, Ord(T))(T a, T b) {                          \
    return _le_(b, a);                                                   \
  }                                                                      \
  static bool FUNC_NAME(gt, Ord(T))(T a, T b) {                          \
    return FUNC_NAME(lt, Ord(T))(b, a);                                  \
  }                                                                      \
  static T FUNC_NAME(min, Ord(T))(T a, T b) {                            \
    return _le_(a, b) ? a : b;                                           \
  }                                                                      \
  static T FUNC_NAME(max, Ord(T))(T a, T b) {                            \
    return _le_(b, a) ? a : b;                                           \
  }                                                                      \
  Ord(T) Trait(Ord(T)) {                                                 \
    return (Ord(T)){                                                     \
        .cmp = FUNC_NAME(cmp, Ord(T)),                                   \
        .lt = FUNC_NAME(lt, Ord(T)),                                     \
        .le = _le_,                                                      \
        .gt = FUNC_NAME(gt, Ord(T)),                                     \
        .ge = FUNC_NAME(ge, Ord(T)),                                     \
        .min = FUNC_NAME(min, Ord(T)),                                   \
        .max = FUNC_NAME(max, Ord(T)),                                   \
    };                                                                   \
  }                                                                      \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Ord_Maybe(T)                                                \
  static int FUNC_NAME(cmp, Ord(Maybe(T)))(Maybe(T) a, Maybe(T) b) {     \
    if (a.none && b.none) {                                              \
      return 0;                                                          \
    }                                                                    \
    if (a.none) {                                                        \
      return -1;                                                         \
    }                                                                    \
    if (b.none) {                                                        \
      return 1;                                                          \
    }                                                                    \
    return trait(Ord(T)).cmp(a.value, b.value);                          \
  }                                                                      \
  instance_Ord(Maybe(T), FUNC_NAME(cmp, Ord(Maybe(T))));                 \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Ord_Array(T)                                                \
  static int FUNC_NAME(cmp, Ord(Array(T)))(Array(T) a, Array(T) b) {     \
    if (a.data != b.data) {                                              \
      size_t n = (a.length <= b.length ? a.length : b.length);           \
      for (size_t i = 0; i < n; ++i) {                                   \
        int o = trait(Ord(T)).cmp(a.data[i], b.data[i]);                 \
        if (o) {                                                         \
          return o;                                                      \
        }                                                                \
      }                                                                  \
    }                                                                    \
    return trait(Ord(size_t)).cmp(a.length, b.length);                   \
  }                                                                      \
  instance_Ord(Array(T), FUNC_NAME(cmp, Ord(Array(T))));                 \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
#define impl_Ord_List(T)                                                 \
  static int FUNC_NAME(cmp, Ord(List(T)))(List(T) a, List(T) b) {        \
    for (;;) {                                                           \
      if (a == b) {                                                      \
        return 0;                                                        \
      }                                                                  \
      if (!a) {                                                          \
        return -1;                                                       \
      }                                                                  \
      if (!b) {                                                          \
        return 1;                                                        \
      }                                                                  \
      int o = trait(Ord(T)).cmp(a->head, b->head);                       \
      if (o) {                                                           \
        return o;                                                        \
      }                                                                  \
      a = a->tail;                                                       \
      b = b->tail;                                                       \
    }                                                                    \
  }                                                                      \
  instance_Ord(List(T), FUNC_NAME(cmp, Ord(List(T))));                   \
  END_OF_STATEMENTS
