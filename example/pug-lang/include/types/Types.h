/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base.h>

#include "Subst.h"
#include "Type.h"

// -----------------------------------------------------------------------
trait_List(Tyvar);

/** make union list of type variables */
List(Tyvar) unionTyvars(List(Tyvar) as, List(Tyvar) bs);

// -----------------------------------------------------------------------
#define Types(T) TYPE_NAME(Types, T)

#define trait_Types(T)                                                   \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct Types(T) Types(T);                                      \
  struct Types(T) {                                                      \
    /** apply type substitution */                                       \
    T (*subst)(Subst s, T t);                                            \
    /** extract list of type variables */                                \
    List(Tyvar) (*tvarsOf)(T t);                                         \
  };                                                                     \
                                                                         \
  Types(T) Trait(Types(T));                                              \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
trait_List(Type);
trait_Types(Type);
trait_Types(List(Type));

// -----------------------------------------------------------------------
// ---- trait Types(List(T))

#define impl_Types_List(T)                                               \
                                                                         \
  static List(T) FUNC_NAME(subst, Types(List(T)))(Subst s, List(T) t) {  \
    Types(T) S = trait(Types(T));                                        \
    ListT(T) L = trait(List(T));                                         \
    List(T) xs = L.empty;                                                \
    while (t) {                                                          \
      xs = L.cons(S.subst(s, L.head(t)), xs);                            \
      t = L.tail(t);                                                     \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
                                                                         \
  static List(Tyvar) FUNC_NAME(tvarsOf, Types(List(T)))(List(T) t) {     \
    if (!t) {                                                            \
      return NULL;                                                       \
    }                                                                    \
    Types(T) S = trait(Types(T));                                        \
    ListT(T) L = trait(List(T));                                         \
    List(Tyvar) xs = NULL;                                               \
    while (t) {                                                          \
      xs = unionTyvars(xs, S.tvarsOf(L.head(t)));                        \
      t = L.tail(t);                                                     \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
                                                                         \
  Types(List(T)) Trait(Types(List(T))) {                                 \
    return (Types(List(T))){                                             \
        .subst = FUNC_NAME(subst, Types(List(T))),                       \
        .tvarsOf = FUNC_NAME(tvarsOf, Types(List(T))),                   \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
