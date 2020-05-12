/* -*- coding: utf-8-unix -*- */

#include "types/Types.h"

// -----------------------------------------------------------------------
impl_List(Tyvar);

List(Tyvar) unionTyvars(List(Tyvar) as, List(Tyvar) bs) {
  if (!as) {
    return bs;
  }
  if (!bs) {
    return as;
  }
  while (bs) {
    bool found = false;
    for (List(Tyvar) xs = as; xs; xs = xs->tail) {
      if (trait(Eq(Tyvar)).eq(xs->head, bs->head)) {
        found = true;
        break;
      }
    }
    if (found) {
      bs = trait(List(Tyvar)).drop(1, bs);
    } else {
      List(Tyvar) ys = bs;
      bs = bs->tail;
      ys->tail = as;
      as = ys;
    }
  }
  return as;
}

// -----------------------------------------------------------------------
// ---- trait Types(Type)

static Type subst_tvar_to_type(Subst s, Type t) {
  assert(t && t->id == TVAR && "Not a type variable");
  while (s) {
    SubstEntry* e = &s->head;
    if (trait(Eq(Tyvar)).eq(e->tvar, t->tvar)) {
      return e->type;
    }
    s = s->tail;
  }
  return t;
}

static Type FUNC_NAME(subst, Types(Type))(Subst s, Type t) {
  assert(t && "Null pointer");
  switch (t->id) {
  case TVAR:
    return subst_tvar_to_type(s, t);
  case TAPPLY: {
    TypeT T = trait(Type);
    Types(Type) S = trait(Types(Type));
    return T.TAp(S.subst(s, t->lhs), S.subst(s, t->rhs));
  }
  default:
    return t;
  }
}

static List(Tyvar) FUNC_NAME(tvarsOf, Types(Type))(Type t) {
  assert(t && "Null pointer");
  ListT(Tyvar) L = trait(List(Tyvar));
  switch (t->id) {
  case TVAR:
    return L.cons(t->tvar, L.empty);
  case TAPPLY: {
    Types(Type) S = trait(Types(Type));
    return unionTyvars(S.tvarsOf(t->lhs), S.tvarsOf(t->rhs));
  }
  default:
    return L.empty;
  }
}

Types(Type) Trait(Types(Type)) {
  return (Types(Type)){
      .subst = FUNC_NAME(subst, Types(Type)),
      .tvarsOf = FUNC_NAME(tvarsOf, Types(Type)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Types(List(Type))

impl_List(Type);
impl_Types_List(Type);
