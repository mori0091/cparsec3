/* -*- coding: utf-8-unix -*- */

#include "types/Subst.h"
#include "types/Types.h"

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
  Types(Type) S = trait(Types(Type));
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

static Maybe(Subst) FUNC_NAME(merge, Subst)(Subst s1, Subst s2) {
  Types(Type) S = trait(Types(Type));
  TypeT T = trait(Type);
  for (Subst xs = s1; xs; xs = xs->tail) {
    Tyvar x = xs->head.tvar;
    for (Subst ys = s2; ys; ys = ys->tail) {
      Tyvar y = ys->head.tvar;
      if (trait(Eq(Tyvar)).eq(x, y)) {
        Type v = T.TVar(x);
        if (trait(Eq(Type)).neq(S.subst(s1, v), S.subst(s2, v))) {
          return (Maybe(Subst)){.none = true};
        }
      }
    }
  }
  // return s1 ++ s2
  Subst xs = s1;
  while (xs->tail) {
    xs = xs->tail;
  }
  xs->tail = s2;
  return (Maybe(Subst)){.value = s1};
}

SubstT Trait(Subst) {
  return (SubstT){
      .empty = NULL,
      .create = FUNC_NAME(create, Subst),
      .composite = FUNC_NAME(composite, Subst),
      .merge = FUNC_NAME(merge, Subst),
  };
}
