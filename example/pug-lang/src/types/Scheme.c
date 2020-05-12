/* -*- coding: utf-8-unix -*- */

#include "types/Scheme.h"

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

impl_List(Scheme);
impl_Types_List(Scheme);
