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
