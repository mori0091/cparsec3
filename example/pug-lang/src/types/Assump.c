/* -*- coding: utf-8-unix -*- */

#include "types/Assump.h"

// -----------------------------------------------------------------------
// ---- trait Types(Assump)

static Assump FUNC_NAME(subst, Types(Assump))(Subst s, Assump as) {
  as.scheme = trait(Types(Scheme)).subst(s, as.scheme);
  return as;
}

static List(Tyvar) FUNC_NAME(tvarsOf, Types(Assump))(Assump as) {
  return trait(Types(Scheme)).tvarsOf(as.scheme);
}

Types(Assump) Trait(Types(Assump)) {
  return (Types(Assump)){
      .subst = FUNC_NAME(subst, Types(Assump)),
      .tvarsOf = FUNC_NAME(tvarsOf, Types(Assump)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Types(List(Assump))

impl_List(Assump);
impl_Types_List(Assump);

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

static Scheme FUNC_NAME(scheme, Assumption)(List(Assump) as, Type t) {
  Types(Type) S = trait(Types(Type));
  Types(List(Assump)) SA = trait(Types(List(Assump)));
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
  return (Scheme){
      .numTGen = n,
      .type = S.subst(s, t),
  };
}

static Maybe(Scheme)
    FUNC_NAME(lookup, Assumption)(Var var, List(Assump) as) {
  while (as) {
    if (trait(Eq(String)).eq(var.ident, as->head.var.ident)) {
      return (Maybe(Scheme)){.value = as->head.scheme};
    }
    as = as->tail;
  }
  return (Maybe(Scheme)){.none = true};
}

static List(Assump)
    FUNC_NAME(add, Assumption)(Var var, Scheme sc, List(Assump) as) {
  Assump c = {.var = var, .scheme = sc};
  as = trait(List(Assump)).cons(c, as);
  return as;
}

Assumption Trait(Assumption) {
  return (Assumption){
      .scheme = FUNC_NAME(scheme, Assumption),
      .lookup = FUNC_NAME(lookup, Assumption),
      .add = FUNC_NAME(add, Assumption),
  };
}