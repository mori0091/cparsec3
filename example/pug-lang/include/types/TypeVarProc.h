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
#define TypeVarProc(T) TYPE_NAME(TypeVarProc, T)

#define trait_TypeVarProc(T)                                             \
  C_API_BEGIN                                                            \
                                                                         \
  typedef struct TypeVarProc(T) TypeVarProc(T);                          \
  struct TypeVarProc(T) {                                                \
    /** apply type substitution */                                       \
    T (*subst)(Subst s, T t);                                            \
    /** extract list of type variables */                                \
    List(Tyvar) (*tvarsOf)(T t);                                         \
  };                                                                     \
                                                                         \
  TypeVarProc(T) Trait(TypeVarProc(T));                                  \
                                                                         \
  C_API_END                                                              \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
trait_List(Type);
trait_TypeVarProc(Type);
trait_TypeVarProc(List(Type));

// -----------------------------------------------------------------------
// ---- trait TypeVarProc(List(T))

#define impl_TypeVarProc_List(T)                                         \
                                                                         \
  static List(T)                                                         \
      FUNC_NAME(subst, TypeVarProc(List(T)))(Subst s, List(T) t) {       \
    TypeVarProc(T) S = trait(TypeVarProc(T));                            \
    ListT(T) L = trait(List(T));                                         \
    List(T) xs = L.empty;                                                \
    while (t) {                                                          \
      xs = L.cons(S.subst(s, L.head(t)), xs);                            \
      t = L.tail(t);                                                     \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
                                                                         \
  static List(Tyvar)                                                     \
      FUNC_NAME(tvarsOf, TypeVarProc(List(T)))(List(T) t) {              \
    if (!t) {                                                            \
      return NULL;                                                       \
    }                                                                    \
    TypeVarProc(T) S = trait(TypeVarProc(T));                            \
    ListT(T) L = trait(List(T));                                         \
    List(Tyvar) xs = NULL;                                               \
    while (t) {                                                          \
      xs = unionTyvars(xs, S.tvarsOf(L.head(t)));                        \
      t = L.tail(t);                                                     \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
                                                                         \
  TypeVarProc(List(T)) Trait(TypeVarProc(List(T))) {                     \
    return (TypeVarProc(List(T))){                                       \
        .subst = FUNC_NAME(subst, TypeVarProc(List(T))),                 \
        .tvarsOf = FUNC_NAME(tvarsOf, TypeVarProc(List(T))),             \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS
