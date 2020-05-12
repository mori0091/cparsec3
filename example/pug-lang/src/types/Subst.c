/* -*- coding: utf-8-unix -*- */

#include "types/Subst.h"
#include "types/TypeVarProc.h"

#include <inttypes.h>

// -----------------------------------------------------------------------
Tyvar freshTVar(void) {
  static uint32_t n = 0;
  CharBuff b = {0};
  mem_printf(&b, "#%" PRIx32, n++);
  return (Tyvar){.ident = b.data};
}

// -----------------------------------------------------------------------
impl_List(SubstEntry);

// -----------------------------------------------------------------------
// ---- trait Subst

static Subst FUNC_NAME(create, Subst)(Tyvar tvar, Type type) {
  SubstEntry e = {.tvar = tvar, .type = type};
  return trait(List(SubstEntry)).cons(e, NULL);
}

static Subst FUNC_NAME(composite, Subst)(Subst s1, Subst s2) {
  if (!s1) {
    return s2;
  }
  if (!s2) {
    return s1;
  }
  SubstT T = trait(Subst);
  TypeVarProc(Type) S = trait(TypeVarProc(Type));
  Subst s = T.create(s2->head.tvar, S.subst(s1, s2->head.type));
  Subst st = s;
  s2 = s2->tail;
  while (s2) {
    st->tail = T.create(s2->head.tvar, S.subst(s1, s2->head.type));
    st = st->tail;
    s2 = s2->tail;
  }
  st->tail = s1;
  return s;
}

SubstT Trait(Subst) {
  return (SubstT){
      .empty = NULL,
      .create = FUNC_NAME(create, Subst),
      .composite = FUNC_NAME(composite, Subst),
  };
}
