/* -*- coding: utf-8-unix -*- */

#include "types/TypeScheme.h"

// -----------------------------------------------------------------------
// ---- trait TypeVarProc(TypeScheme)

static TypeScheme
FUNC_NAME(subst, TypeVarProc(TypeScheme))(Subst s, TypeScheme t) {
  t.type = trait(TypeVarProc(Type)).subst(s, t.type);
  return t;
}

static List(Tyvar)
    FUNC_NAME(tvarsOf, TypeVarProc(TypeScheme))(TypeScheme t) {
  return trait(TypeVarProc(Type)).tvarsOf(t.type);
}

TypeVarProc(TypeScheme) Trait(TypeVarProc(TypeScheme)) {
  return (TypeVarProc(TypeScheme)){
      .subst = FUNC_NAME(subst, TypeVarProc(TypeScheme)),
      .tvarsOf = FUNC_NAME(tvarsOf, TypeVarProc(TypeScheme)),
  };
}

// -----------------------------------------------------------------------
// ---- trait TypeVarProc(List(TypeScheme))

impl_List(TypeScheme);
impl_TypeVarProc_List(TypeScheme);
