/* -*- coding: utf-8-unix -*- */

#include "types/TypeUnify.h"
#include "types/TypeVarProc.h"

Maybe(Subst) FUNC_NAME(unifier, TypeUnify)(Type t1, Type t2) {
  TypeVarProc(Type) S = trait(TypeVarProc(Type));
  TypeUnify U = trait(TypeUnify);
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

Maybe(Subst) FUNC_NAME(tbind, TypeUnify)(Tyvar tvar, Type t) {
  SubstT TS = trait(Subst);
  Eq(Tyvar) E = trait(Eq(Tyvar));

  /* Is type `t` same as the type variable `tvar` ? */
  if (t->id == TVAR && E.eq(t->tvar, tvar)) {
    return (Maybe(Subst)){.value = TS.empty};
  }
  /* Is type `t` contains same type variable with `tvar` ? */
  {
    List(Tyvar) xs = trait(TypeVarProc(Type)).tvarsOf(t);
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

TypeUnify Trait(TypeUnify) {
  return (TypeUnify){
      .unifier = FUNC_NAME(unifier, TypeUnify),
      .tbind = FUNC_NAME(tbind, TypeUnify),
  };
}
