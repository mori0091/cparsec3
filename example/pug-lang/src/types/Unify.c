/* -*- coding: utf-8-unix -*- */

#include "types/Unify.h"
#include "types/Types.h"

Maybe(Subst) FUNC_NAME(unifier, Unify)(Type t1, Type t2) {
  Types(Type) S = trait(Types(Type));
  Unify U = trait(Unify);
  if (t1->id == TAPPLY && t2->id == TAPPLY) {
    Maybe(Subst) s1, s2;
    s1 = U.unifier(t1->lhs, t2->lhs);
    if (s1.none) {
      return s1; /* error */
    }
    s2 =
        U.unifier(S.subst(s1.value, t1->rhs), S.subst(s1.value, t2->rhs));
    if (s2.none) {
      return s2; /* error */
    }
    SubstT TS = trait(Subst);
    return (Maybe(Subst)){.value = TS.composite(s2.value, s1.value)};
  }
  if (t1->id == TVAR) {
    return U.tbind(t1->tvar, t2);
  }
  if (t2->id == TVAR) {
    return U.tbind(t2->tvar, t1);
  }
  if (t1->id == TCON && t2->id == TCON && trait(Eq(Type)).eq(t1, t2)) {
    return (Maybe(Subst)){.value = NULL};
  }
  /* otherwise error */
  return (Maybe(Subst)){.none = true};
}

Maybe(Subst) FUNC_NAME(tbind, Unify)(Tyvar tvar, Type t) {
  SubstT TS = trait(Subst);
  Eq(Tyvar) E = trait(Eq(Tyvar));

  /* Is type `t` same as the type variable `tvar` ? */
  if (t->id == TVAR && E.eq(t->tvar, tvar)) {
    return (Maybe(Subst)){.value = TS.empty};
  }
  /* Is type `t` contains same type variable with `tvar` ? */
  {
    List(Tyvar) xs = trait(Types(Type)).tvarsOf(t);
    while (xs) {
      if (E.eq(xs->head, tvar)) {
        return (Maybe(Subst)){.none = true}; /* error */
      }
      xs = xs->tail;
    }
  }
  /* otherwise */
  return (Maybe(Subst)){.value = TS.create(tvar, t)};
}

Unify Trait(Unify) {
  return (Unify){
      .unifier = FUNC_NAME(unifier, Unify),
      .tbind = FUNC_NAME(tbind, Unify),
  };
}
