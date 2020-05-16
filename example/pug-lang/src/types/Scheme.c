/* -*- coding: utf-8-unix -*- */

#include "types/Scheme.h"

Scheme Forall(List(Kind) ks, Qual(Type) qt) {
  return (Scheme){ks, qt};
}

static bool elem(Tyvar v, List(Tyvar) vs) {
  Eq(Tyvar) E = trait(Eq(Tyvar));
  while (vs) {
    if (E.eq(v, vs->head)) {
      return true;
    }
    vs = vs->tail;
  }
  return false;
}

static List(Tyvar) intersectTyvars(List(Tyvar) vs1, List(Tyvar) vs2) {
  ListT(Tyvar) L = trait(List(Tyvar));
  while (vs1 && !elem(vs1->head, vs2)) {
    vs1 = L.drop(1, vs1);
  }
  for (List(Tyvar) xs = vs1; xs; xs = xs->tail) {
    while (xs->tail && !elem(xs->tail->head, vs2)) {
      xs->tail = L.drop(1, xs->tail);
    }
  }
  return vs1;
}

List(Kind) mapKind(List(Tyvar) vs) {
  if (!vs) {
    return NULL;
  }
  Kind k = trait(HasKind(Tyvar)).kind(vs->head);
  return trait(List(Kind)).cons(k, mapKind(vs->tail));
}

Subst toSubst(List(Tyvar) vs) {
  ListT(SubstEntry) L = trait(List(SubstEntry));
  TypeT T = trait(Type);
  Subst s = 0;
  int n = 0;
  while (vs) {
    SubstEntry x = {.tvar = vs->head, .type = T.TGen(n)};
    s = L.cons(x, s);
    vs = vs->tail;
    n++;
  }
  L.reverse(&s);
  return s;
}

Scheme quantify(List(Tyvar) vs, Qual(Type) qt) {
  // ---- vs1 = [v | v <- tv qt, v ‘elem‘ vs]
  List(Tyvar) vs1 = trait(Types(Qual(Type))).tvarsOf(qt);
  vs1 = intersectTyvars(vs1, vs);
  // ---- ks = map kind vs1
  List(Kind) ks = mapKind(vs1);
  // ---- s = zip vs1 (map TGen [0..])
  Subst s = toSubst(vs1);
  // ---- Forall ks (apply s qt)
  return Forall(ks, trait(Types(Qual(Type))).subst(s, qt));
}

Scheme toScheme(Type t) {
  return Forall(NULL, (Qual(Type)){NULL, t});
}

// -----------------------------------------------------------------------
// ---- trait Eq(Scheme)
static bool FUNC_NAME(eq, Eq(Scheme))(Scheme a, Scheme b) {
  return trait(Eq(List(Kind))).eq(a.ks, b.ks) &&
         trait(Eq(Qual(Type))).eq(a.qt, b.qt);
}
instance_Eq(Scheme, FUNC_NAME(eq, Eq(Scheme)));

// -----------------------------------------------------------------------
// ---- trait List(Scheme)
impl_List(Scheme);

// -----------------------------------------------------------------------
// ---- trait Eq(List(Scheme))
impl_Eq_List(Scheme);

// -----------------------------------------------------------------------
// ---- trait Types(Scheme)
static Scheme FUNC_NAME(subst, Types(Scheme))(Subst s, Scheme t) {
  t.qt = trait(Types(Qual(Type))).subst(s, t.qt);
  return t;
}

static List(Tyvar) FUNC_NAME(tvarsOf, Types(Scheme))(Scheme t) {
  return trait(Types(Qual(Type))).tvarsOf(t.qt);
}

Types(Scheme) Trait(Types(Scheme)) {
  return (Types(Scheme)){
      .subst = FUNC_NAME(subst, Types(Scheme)),
      .tvarsOf = FUNC_NAME(tvarsOf, Types(Scheme)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Types(List(Scheme))
impl_Types_List(Scheme);
