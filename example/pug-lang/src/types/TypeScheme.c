/* -*- coding: utf-8-unix -*- */

#include "types/TypeScheme.h"

// -----------------------------------------------------------------------
// ---- trait Types(TypeScheme)

static TypeScheme FUNC_NAME(subst, Types(TypeScheme))(Subst s,
                                                      TypeScheme t) {
  t.type = trait(Types(Type)).subst(s, t.type);
  return t;
}

static List(Tyvar) FUNC_NAME(tvarsOf, Types(TypeScheme))(TypeScheme t) {
  return trait(Types(Type)).tvarsOf(t.type);
}

Types(TypeScheme) Trait(Types(TypeScheme)) {
  return (Types(TypeScheme)){
      .subst = FUNC_NAME(subst, Types(TypeScheme)),
      .tvarsOf = FUNC_NAME(tvarsOf, Types(TypeScheme)),
  };
}

// -----------------------------------------------------------------------
// ---- trait Types(List(TypeScheme))

impl_List(TypeScheme);
impl_Types_List(TypeScheme);
