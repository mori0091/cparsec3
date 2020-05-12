/* -*- coding: utf-8-unix -*- */

#include "types/TypeAssumption.h"

// -----------------------------------------------------------------------
// ---- trait Types(TypeAssumption)

static TypeAssumption
FUNC_NAME(subst, Types(TypeAssumption))(Subst s, TypeAssumption as) {
  as.scheme = trait(Types(TypeScheme)).subst(s, as.scheme);
  return as;
}

static List(Tyvar)
    FUNC_NAME(tvarsOf, Types(TypeAssumption))(TypeAssumption as) {
  return trait(Types(TypeScheme)).tvarsOf(as.scheme);
}

Types(TypeAssumption) Trait(Types(TypeAssumption)) {
  return (Types(TypeAssumption)){
      .subst = FUNC_NAME(subst, Types(TypeAssumption)),
      .tvarsOf = FUNC_NAME(tvarsOf, Types(TypeAssumption)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Types(List(TypeAssumption))

impl_List(TypeAssumption);
impl_Types_List(TypeAssumption);

// -----------------------------------------------------------------------
// ---- trait Assumption

List(Tyvar) subtractTyvars(List(Tyvar) a, List(Tyvar) b) {
  if (!a) {
    return NULL;
  }
  if (!b) {
    return a;
  }
  ListT(Tyvar) L = trait(List(Tyvar));
  Eq(Tyvar) E = trait(Eq(Tyvar));
  while (b) {
    if (E.eq(a->head, b->head)) {
      /* a = drop(1, a); */
      a = L.tail(a);
    } else {
      List(Tyvar) xs = a;
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
  Types(Type) S = trait(Types(Type));
  Types(List(TypeAssumption)) SA = trait(Types(List(TypeAssumption)));
  List(Tyvar) gs = subtractTyvars(S.tvarsOf(t), SA.tvarsOf(as));
  // int n = trait(List(Tyvar)).length(gs);
  TypeT T = trait(Type);
  ListT(SubstEntry) L = trait(List(SubstEntry));
  Subst s = 0;
  int n = 0;
  while (gs) {
    SubstEntry x = {.tvar = gs->head, .type = T.TGen(n)};
    s = L.cons(x, s);
    gs = gs->tail;
    n++;
  }
  L.reverse(&s);
  return (TypeScheme){
      .numTGen = n,
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

static List(TypeAssumption)
    FUNC_NAME(add, Assumption)(Var var, TypeScheme sc,
                               List(TypeAssumption) as) {
  TypeAssumption c = {.var = var, .scheme = sc};
  as = trait(List(TypeAssumption)).cons(c, as);
  return as;
}

Assumption Trait(Assumption) {
  return (Assumption){
      .scheme = FUNC_NAME(scheme, Assumption),
      .lookup = FUNC_NAME(lookup, Assumption),
      .add = FUNC_NAME(add, Assumption),
  };
}
