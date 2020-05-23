/* -*- coding: utf-8-unix -*- */

#include "types/Instantiate.h"

// -----------------------------------------------------------------------
// ---- trait Instantiate(List(A))
#define impl_Instantiate_List(A)                                         \
  static List(A)                                                         \
      FUNC_NAME(inst, Instantiate(List(A)))(List(Type) ts, List(A) as) { \
    if (!as) {                                                           \
      return NULL;                                                       \
    }                                                                    \
                                                                         \
    ListT(A) L = trait(List(A));                                         \
    Instantiate(A) I = trait(Instantiate(A));                            \
                                                                         \
    List(A) xs = L.cons(I.inst(ts, as->head), NULL);                     \
    {                                                                    \
      List(A) ys = xs;                                                   \
      as = as->tail;                                                     \
      while (as) {                                                       \
        ys = ys->tail = L.cons(I.inst(ts, as->head), NULL);              \
        as = as->tail;                                                   \
      }                                                                  \
    }                                                                    \
    return xs;                                                           \
  }                                                                      \
                                                                         \
  Instantiate(List(A)) Trait(Instantiate(List(A))) {                     \
    return (Instantiate(List(A))){                                       \
        .inst = FUNC_NAME(inst, Instantiate(List(A))),                   \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
// ---- trait Instantiate(Qual(T))
#define impl_Instantiate_Qual(T)                                         \
                                                                         \
  static Qual(T)                                                         \
      FUNC_NAME(inst, Instantiate(Qual(T)))(List(Type) ts, Qual(T) qt) { \
    Instantiate(T) I = trait(Instantiate(T));                            \
    Instantiate(List(Pred)) P = trait(Instantiate(List(Pred)));          \
    qt.ps = P.inst(ts, qt.ps);                                           \
    qt.t = I.inst(ts, qt.t);                                             \
    return qt;                                                           \
  }                                                                      \
                                                                         \
  Instantiate(Qual(T)) Trait(Instantiate(Qual(T))) {                     \
    return (Instantiate(Qual(T))){                                       \
        .inst = FUNC_NAME(inst, Instantiate(Qual(T))),                   \
    };                                                                   \
  }                                                                      \
                                                                         \
  END_OF_STATEMENTS

// -----------------------------------------------------------------------
// ---- trait Instantiate(Type)
static Type FUNC_NAME(inst, Instantiate(Type))(List(Type) ts, Type t) {
  Instantiate(Type) I = trait(Instantiate(Type));
  switch (t->id) {
  case TAPPLY:
    return trait(Type).TAp(I.inst(ts, t->lhs), I.inst(ts, t->rhs));
  case TGEN:;
    for (size_t n = t->tgen.n; n; n--) {
      ts = ts->tail;
    }
    return ts->head;
  default:
    return t;
  }
}

Instantiate(Type) Trait(Instantiate(Type)) {
  return (Instantiate(Type)){
      .inst = FUNC_NAME(inst, Instantiate(Type)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Instantiate(List(Type))
impl_Instantiate_List(Type);

// -----------------------------------------------------------------------
// ---- trait Instantiate(Qual(Type))
impl_Instantiate_Qual(Type);

// -----------------------------------------------------------------------
// ---- trait Instantiate(Qual(List(Type)))
impl_Instantiate_Qual(List(Type));

// -----------------------------------------------------------------------
// ---- trait Instantiate(List(Qual(Type)))
// impl_Instantiate_List(Qual(Type));

// -----------------------------------------------------------------------
// ---- trait Instantiate(List(Qual(List(Type))))
// impl_Instantiate_List(Qual(List(Type)));

// -----------------------------------------------------------------------
// ---- trait Instantiate(Pred)
static Pred FUNC_NAME(inst, Instantiate(Pred))(List(Type) ts, Pred p) {
  Instantiate(Type) I = trait(Instantiate(Type));
  return IsIn(p.ident, I.inst(ts, p.type));
}

Instantiate(Pred) Trait(Instantiate(Pred)) {
  return (Instantiate(Pred)){
      .inst = FUNC_NAME(inst, Instantiate(Pred)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Instantiate(List(Pred))
impl_Instantiate_List(Pred);
