/* -*- coding: utf-8-unix -*- */

#include "types/TypeInfer.h"

// -----------------------------------------------------------------------
Infered(Type) InferedType(List(Pred) ps, Type t) {
  return (Infered(Type)){.ps = ps, .t = t};
}

// -----------------------------------------------------------------------
List(Pred) appendPreds(List(Pred) ps1, List(Pred) ps2) {
  if (!ps1) {
    return ps2;
  }
  if (!ps2) {
    return ps1;
  }
  List(Pred) xs = ps1;
  while (xs->tail) {
    xs = xs->tail;
  }
  xs->tail = ps2;
  return ps1;
}

// -----------------------------------------------------------------------
action(getSubst, Subst) {
  A_DO() {
    A_RUN(getState(), s);
    A_RETURN(s.subst);
  }
}

// -----------------------------------------------------------------------
action(extSubst, Subst, None) {
  A_DO_WITH(sub) {
    A_RUN(getState(), s);
    s.subst = t_composite_subst(sub, s.subst);
    A_RUN(putState(s));
    A_RETURN((None){0});
  }
}

// -----------------------------------------------------------------------
action(unify, Type, Type, None) {
  A_DO_WITH(t1, t2) {
    A_RUN(getState(), s);
    Maybe(Subst) u =
        t_unifier(t_apply_subst(s.subst, t1), t_apply_subst(s.subst, t2));
    if (u.none) {
      Show(Type) S = trait(Show(Type));
      CharBuff b = {0};
      mem_printf(&b, "Type mismatch\n  t1 = ");
      S.toString(&b, t1);
      mem_printf(&b, "\n  t2 = ");
      S.toString(&b, t2);
      TypeError e = {b.data};
      A_FAIL(e);
    }
    A_RUN(extSubst(u.value));
    A_RETURN((None){0});
  }
}

// -----------------------------------------------------------------------
action(newTVar, Kind, Type) {
  A_DO_WITH(k) {
    A_RUN(getState(), s);
    CharBuff b = {0};
    mem_printf(&b, "#a%d", s.i++);
    Type t = trait(Type).TVar((Tyvar){b.data, k});
    A_RUN(putState(s));
    A_RETURN(t);
  }
}

// -----------------------------------------------------------------------
action(freshInst, Scheme, Qual(Type)) {
  A_DO_WITH(sc) {
    List(Type) ts = NULL;
    if (sc.ks) {
      ListT(Type) L = trait(List(Type));
      for (List(Kind) ks = sc.ks; ks; ks = ks->tail) {
        A_RUN(newTVar(ks->head), x);
        ts = L.cons(x, ts);
      }
      L.reverse(&ts);
    }
    Instantiate(Qual(Type)) I = trait(Instantiate(Qual(Type)));
    A_RETURN(I.inst(ts, sc.qt));
  }
}
