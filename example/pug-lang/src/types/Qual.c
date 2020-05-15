/* -*- coding: utf-8-unix -*- */

#include "types/Qual.h"

#define impl_Qual(T)                                                     \
  static Qual(T) FUNC_NAME(create, Qual(T))(List(Pred) ps, T t) {        \
    return (Qual(T)){ps, t};                                             \
  }                                                                      \
  QualT(T) Trait(Qual(T)) {                                              \
    return (QualT(T)){                                                   \
        .create = FUNC_NAME(create, Qual(T)),                            \
    };                                                                   \
  }                                                                      \
                                                                         \
  static bool FUNC_NAME(eq, Eq(Qual(T)))(Qual(T) a, Qual(T) b) {         \
    return trait(Eq(List(Pred))).eq(a.ps, b.ps) &&                       \
           trait(Eq(T)).eq(a.t, b.t);                                    \
  }                                                                      \
  instance_Eq(Qual(T), FUNC_NAME(eq, Eq(Qual(T))));                      \
                                                                         \
  static Qual(T) FUNC_NAME(subst, Types(Qual(T)))(Subst s, Qual(T) q) {  \
    List(Pred) ps = trait(Types(List(Pred))).subst(s, q.ps);             \
    T t = trait(Types(T)).subst(s, q.t);                                 \
    return (Qual(T)){ps, t};                                             \
  }                                                                      \
  static List(Tyvar) FUNC_NAME(tvarsOf, Types(Qual(T)))(Qual(T) q) {     \
    return unionTyvars(trait(Types(List(Pred))).tvarsOf(q.ps),           \
                       trait(Types(T)).tvarsOf(q.t));                    \
  }                                                                      \
  Types(Qual(T)) Trait(Types(Qual(T))) {                                 \
    return (Types(Qual(T))){                                             \
        .subst = FUNC_NAME(subst, Types(Qual(T))),                       \
        .tvarsOf = FUNC_NAME(tvarsOf, Types(Qual(T))),                   \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

impl_Qual(Type);
impl_Qual(Scheme);
impl_Qual(Assump);

impl_Qual(List(Type));
impl_Qual(List(Scheme));
impl_Qual(List(Assump));
