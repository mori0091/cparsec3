/* -*- coding: utf-8-unix -*- */

#include "types/TypeSubst.h"
#include "types/TypeVarProc.h"

#include <inttypes.h>

// -----------------------------------------------------------------------
TVar freshTVar(void) {
  static uint32_t n = 0;
  CharBuff b = {0};
  mem_printf(&b, "#%" PRIx32, n++);
  return (TVar){.ident = b.data};
}

// -----------------------------------------------------------------------
impl_List(TypeSubstEntry);

// -----------------------------------------------------------------------
// ---- trait TypeSubst

static TypeSubst FUNC_NAME(create, TypeSubst)(TVar tvar, Type type) {
  TypeSubstEntry e = {.tvar = tvar, .type = type};
  return trait(List(TypeSubstEntry)).cons(e, NULL);
}

static TypeSubst FUNC_NAME(composite, TypeSubst)(TypeSubst s1,
                                                 TypeSubst s2) {
  if (!s1) {
    return s2;
  }
  if (!s2) {
    return s1;
  }
  TypeSubstT T = trait(TypeSubst);
  TypeVarProc(Type) S = trait(TypeVarProc(Type));
  TypeSubst s = T.create(s2->head.tvar, S.subst(s1, s2->head.type));
  TypeSubst st = s;
  s2 = s2->tail;
  while (s2) {
    st->tail = T.create(s2->head.tvar, S.subst(s1, s2->head.type));
    st = st->tail;
    s2 = s2->tail;
  }
  st->tail = s1;
  return s;
}

TypeSubstT Trait(TypeSubst) {
  return (TypeSubstT){
      .empty = NULL,
      .create = FUNC_NAME(create, TypeSubst),
      .composite = FUNC_NAME(composite, TypeSubst),
  };
}
