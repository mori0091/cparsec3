/* -*- coding: utf-8-unix -*- */

#include "types/TypeAssumption.h"

// -----------------------------------------------------------------------
// ---- trait TypeVarProc(TypeAssumption)

static TypeAssumption
FUNC_NAME(subst, TypeVarProc(TypeAssumption))(TypeSubst s,
                                              TypeAssumption as) {
  as.scheme = trait(TypeVarProc(TypeScheme)).subst(s, as.scheme);
  return as;
}

static List(TVar)
    FUNC_NAME(tvarsOf, TypeVarProc(TypeAssumption))(TypeAssumption as) {
  return trait(TypeVarProc(TypeScheme)).tvarsOf(as.scheme);
}

TypeVarProc(TypeAssumption) Trait(TypeVarProc(TypeAssumption)) {
  return (TypeVarProc(TypeAssumption)){
      .subst = FUNC_NAME(subst, TypeVarProc(TypeAssumption)),
      .tvarsOf = FUNC_NAME(tvarsOf, TypeVarProc(TypeAssumption)),
  };
}

// -----------------------------------------------------------------------
// ---- trait TypeVarProc(List(TypeAssumption))

impl_List(TypeAssumption);
impl_TypeVarProc_List(TypeAssumption);

// -----------------------------------------------------------------------
// ---- trait Assumption

List(TVar) subtractTVars(List(TVar) a, List(TVar) b) {
  if (!a) {
    return NULL;
  }
  if (!b) {
    return a;
  }
  ListT(TVar) L = trait(List(TVar));
  Eq(TVar) E = trait(Eq(TVar));
  while (b) {
    if (E.eq(a->head, b->head)) {
      /* a = drop(1, a); */
      a = L.tail(a);
    } else {
      List(TVar) xs = a;
      while (xs->tail) {
        if (E.eq(xs->tail->head, b->head)) {
          /* xs->tail = L.drop(1, xs->tail); */
          xs->tail = xs->tail->tail;
          break;
        }
        xs = xs->tail;
      }
    }
    b = b->tail;
  }
  return a;
}

static TypeScheme FUNC_NAME(scheme, Assumption)(List(TypeAssumption) as,
                                                Type t) {
  TypeVarProc(Type) S = trait(TypeVarProc(Type));
  TypeVarProc(List(TypeAssumption)) SA =
      trait(TypeVarProc(List(TypeAssumption)));
  List(TVar) gs = subtractTVars(S.tvarsOf(t), SA.tvarsOf(as));
  // int n = trait(List(TVar)).length(gs);
  TypeT T = trait(Type);
  ListT(TypeSubstEntry) L = trait(List(TypeSubstEntry));
  TypeSubst s = 0;
  int n = 0;
  while (gs) {
    TypeSubstEntry x = {.tvar = gs->head, .type = T.any((TAny){n})};
    s = L.cons(x, s);
    gs = gs->tail;
    n++;
  }
  L.reverse(&s);
  return (TypeScheme){
      .numAny = n,
      .type = S.subst(s, t),
  };
}

static Maybe(TypeScheme)
    FUNC_NAME(lookup, Assumption)(Var var, List(TypeAssumption) as) {
  while (as) {
    if (trait(Eq(String)).eq(var.ident, as->head.var.ident)) {
      return (Maybe(TypeScheme)){.value = as->head.scheme};
    }
    as = as->tail;
  }
  return (Maybe(TypeScheme)){.none = true};
}

Assumption Trait(Assumption) {
  return (Assumption){
      .scheme = FUNC_NAME(scheme, Assumption),
      .lookup = FUNC_NAME(lookup, Assumption),

  };
}
