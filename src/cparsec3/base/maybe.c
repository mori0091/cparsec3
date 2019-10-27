/* -*- coding: utf-8-unix -*- */

#include <cparsec3/base/base.h>

#define EQ(T) CONCAT(eq, T)
#define NEQ(T) CONCAT(neq, T)
#define _EQ_(T) cparsec_module(Eq(T)).eq
#define _NEQ_(T) cparsec_module(Eq(T)).neq

#define define_Eq_Maybe(T)                                               \
  static bool EQ(Maybe(T))(Maybe(T) a, Maybe(T) b) {                     \
    if (a.none && b.none) {                                              \
      return true;                                                       \
    }                                                                    \
    if (a.none != b.none) {                                              \
      return false;                                                      \
    }                                                                    \
    return _EQ_(T)(a.value, b.value);                                    \
  }                                                                      \
  static bool NEQ(Maybe(T))(Maybe(T) a, Maybe(T) b) {                    \
    if (a.none && b.none) {                                              \
      return false;                                                      \
    }                                                                    \
    if (a.none != b.none) {                                              \
      return true;                                                       \
    }                                                                    \
    return _NEQ_(T)(a.value, b.value);                                   \
  }                                                                      \
  Eq(Maybe(T)) CPARSEC_MODULE(Eq(Maybe(T)))(void) {                      \
    return (Eq(Maybe(T))){.eq = EQ(Maybe(T)), .neq = NEQ(Maybe(T))};     \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(define_Eq_Maybe, TYPESET(ALL));

// -----------------------------------------------------------------------

#define CMP(T) CONCAT(cmp, T)
#define LT(T) CONCAT(lt, T)
#define LE(T) CONCAT(le, T)
#define GT(T) CONCAT(gt, T)
#define GE(T) CONCAT(ge, T)
#define MIN(T) CONCAT(min, T)
#define MAX(T) CONCAT(max, T)

#define _LE_(T) cparsec_module(Ord(T)).le

#define define_Ord_Maybe(T)                                              \
  static bool LE(Maybe(T))(Maybe(T) a, Maybe(T) b) {                     \
    if (a.none) {                                                        \
      return true;                                                       \
    }                                                                    \
    if (b.none) {                                                        \
      return false;                                                      \
    }                                                                    \
    return _LE_(T)(a.value, b.value);                                    \
  }                                                                      \
  static bool LT(Maybe(T))(Maybe(T) a, Maybe(T) b) {                     \
    return LE(Maybe(T))(a, b) && !EQ(Maybe(T))(a, b);                    \
  }                                                                      \
  static bool GE(Maybe(T))(Maybe(T) a, Maybe(T) b) {                     \
    return LE(Maybe(T))(b, a);                                           \
  }                                                                      \
  static bool GT(Maybe(T))(Maybe(T) a, Maybe(T) b) {                     \
    return LT(Maybe(T))(b, a);                                           \
  }                                                                      \
  static int CMP(Maybe(T))(Maybe(T) a, Maybe(T) b) {                     \
    return (LE(Maybe(T))(a, b) ? (EQ(Maybe(T))(a, b) ? 0 : -1) : 1);     \
  }                                                                      \
  static Maybe(T) MIN(Maybe(T))(Maybe(T) a, Maybe(T) b) {                \
    return LE(Maybe(T))(a, b) ? a : b;                                   \
  }                                                                      \
  static Maybe(T) MAX(Maybe(T))(Maybe(T) a, Maybe(T) b) {                \
    return LE(Maybe(T))(b, a) ? a : b;                                   \
  }                                                                      \
  Ord(Maybe(T)) CPARSEC_MODULE(Ord(Maybe(T)))(void) {                    \
    return (Ord(Maybe(T))){.cmp = CMP(Maybe(T)),                         \
                           .lt = LT(Maybe(T)),                           \
                           .le = LE(Maybe(T)),                           \
                           .gt = GT(Maybe(T)),                           \
                           .ge = GE(Maybe(T)),                           \
                           .min = MIN(Maybe(T)),                         \
                           .max = MAX(Maybe(T))};                        \
  }                                                                      \
  END_OF_STATEMENTS

FOREACH(define_Ord_Maybe, TYPESET(ALL));
