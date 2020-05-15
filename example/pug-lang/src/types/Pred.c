/* -*- coding: utf-8-unix -*- */

#include "types/Pred.h"

Pred IsIn(String ident, Type type) {
  return (Pred){ident, type};
}

// trait_Eq(Pred);
static bool FUNC_NAME(eq, Eq(Pred))(Pred p1, Pred p2) {
  return (trait(Eq(String)).eq(p1.ident, p2.ident) &&
          trait(Eq(Type)).eq(p1.type, p2.type));
}
instance_Eq(Pred, FUNC_NAME(eq, Eq(Pred)));

// trait_List(Pred);
impl_List(Pred);

// trait_Eq(List(Pred));
impl_Eq_List(Pred);

// trait_Types(Pred);
static Pred FUNC_NAME(subst, Types(Pred))(Subst s, Pred p) {
  return IsIn(p.ident, trait(Types(Type)).subst(s, p.type));
}
static List(Tyvar) FUNC_NAME(tvarsOf, Types(Pred))(Pred p) {
  return trait(Types(Type)).tvarsOf(p.type);
}
Types(Pred) Trait(Types(Pred)) {
  return (Types(Pred)){
      .subst = FUNC_NAME(subst, Types(Pred)),
      .tvarsOf = FUNC_NAME(tvarsOf, Types(Pred)),
  };
}

// trait_Types(List(Pred));
impl_Types_List(Pred);
