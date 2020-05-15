/* -*- coding: utf-8-unix -*- */

#include "types/Scheme.h"

// -----------------------------------------------------------------------
// ---- trait Eq(Scheme)
static bool FUNC_NAME(eq, Eq(Scheme))(Scheme a, Scheme b) {
  return a.numTGen == b.numTGen && trait(Eq(Type)).eq(a.type, b.type);
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
  t.type = trait(Types(Type)).subst(s, t.type);
  return t;
}

static List(Tyvar) FUNC_NAME(tvarsOf, Types(Scheme))(Scheme t) {
  return trait(Types(Type)).tvarsOf(t.type);
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
