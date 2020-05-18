/* -*- coding: utf-8-unix -*- */

#include "types/TypeInfer.h"

static inline Infered(Type) InferedType(List(Pred) ps, Type t) {
  return (Infered(Type)){.ps = ps, .t = t};
}

static List(Pred) appendPreds(List(Pred) ps1, List(Pred) ps2) {
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

// -----------------------------------------------------------------------
action(ti_label0, TI(Infered(Type)), Expr, Infered(Type)) {
  A_DO_WITH(ti, e) {
    A_RUN(getState(), s);
    TIResult(Infered(Type)) r = runTI(ti, s);
    A_RUN(putState(r.state));
    if (r.success) {
      A_RETURN(r.ok);
    }
    CharBuff b = {0};
    mem_printf(&b, "%s\n | %s", r.err.msg, trait(Show(Expr)).show(e));
    r.err.msg = b.data;
    A_FAIL(r.err);
  }
}

TI(Infered(Type)) ti_label(TI(Infered(Type)) ti, Expr e) {
  switch (e->id) {
  case SEQ:
  case BLK:
    return ti;
  default:
    return ti_label0(ti, e);
  }
}

// -----------------------------------------------------------------------
action(typeOfVar, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Maybe(Scheme) sc = t_find(e->ident, as);
    if (sc.none) {
      CharBuff b = {0};
      mem_printf(&b, "Undefined variable - %s", e->ident);
      A_FAIL((TypeError){b.data});
    }
    A_RUN(freshInst(sc.value), qt);
    A_RETURN(InferedType(qt.ps, qt.t));
  }
}

action(typeOfLambda, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    A_RUN(newTVar(trait(Kind).Star()), a);
    A_RUN(newTVar(trait(Kind).Star()), b);
    A_RUN(newTVar(trait(Kind).Star()), f);
    A_RUN(unify(T.func(a, b), f));
    Scheme sc = toScheme(a); // TODO !?
    as = t_add(e->lhs->ident, sc, as);
    A_RUN(typeOf(as, e->rhs), c);
    A_RUN(unify(c.t, b));
    A_RETURN(InferedType(NULL, f));
  }
}

action(typeOfApply, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    A_RUN(typeOf(as, e->lhs), a);
    A_RUN(typeOf(as, e->rhs), b);
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(T.func(b.t, t), a.t));
    List(Pred) ps = appendPreds(a.ps, b.ps);
    A_RETURN(InferedType(ps, t));
  }
}

action(typeOfIfelse, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(typeOf(as, e->lhs), c);
    A_RUN(unify(c.t, Bool));
    A_RUN(typeOf(as, e->rhs->lhs), a);
    A_RUN(typeOf(as, e->rhs->rhs), b);
    A_RUN(unify(a.t, b.t));
    List(Pred) ps = appendPreds(a.ps, b.ps);
    A_RETURN(InferedType(ps, a.t));
  }
}

action(typeOfBlk, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    A_RUN(typeOf(as, e->rhs), x);
    A_RETURN(x);
  }
}

action(typeOfSeq, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    switch (e->lhs->id) {
    case DECLVAR: {
      Scheme sc = t_gen(as, e->lhs->rhs->texpr);
      as = t_add(e->lhs->lhs->ident, sc, as);
      A_RUN(typeOf(as, e->rhs), x);
      A_RETURN(x);
    }
    case LET: {
      Maybe(Scheme) sc = t_find(e->lhs->lhs->ident, as);
      if (!sc.none) {
        A_RUN(typeOf(as, e->lhs->rhs), a);
        A_RUN(getSubst(), sub);
        Scheme sc = t_gen(as, t_apply_subst(sub, a.t));
        as = t_add(e->lhs->lhs->ident, sc, as);
        A_RUN(typeOf(as, e->rhs), x);
        List(Pred) ps = appendPreds(a.ps, x.ps);
        A_RETURN(InferedType(ps, x.t));
      } else {
        if (e->lhs->rhs->id == LAMBDA) {
          A_RUN(newTVar(trait(Kind).Star()), a);
          A_RUN(newTVar(trait(Kind).Star()), b);
          A_RUN(newTVar(trait(Kind).Star()), f);
          A_RUN(unify(T.func(a, b), f));
          Scheme sc = toScheme(f); // TODO !?
          as = t_add(e->lhs->lhs->ident, sc, as);
        }
        A_RUN(typeOf(as, e->lhs->rhs), a);
        A_RUN(getSubst(), sub);
        Scheme sc = t_gen(as, t_apply_subst(sub, a.t));
        as = t_add(e->lhs->lhs->ident, sc, as);
        A_RUN(typeOf(as, e->rhs), x);
        A_RETURN(x);
      }
    }
    default: {
      A_RUN(typeOf(as, e->lhs));
      A_RUN(typeOf(as, e->rhs), t);
      A_RETURN(t);
    }
    }
  }
}

action(typeOfDeclvar, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    // TODO what should we do here?
    A_RETURN(InferedType(NULL, e->rhs->texpr));
  }
}

action(typeOfAssign, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    A_RUN(typeOf(as, e->lhs), a);
    A_RUN(typeOf(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    List(Pred) ps = appendPreds(a.ps, b.ps);
    A_RETURN(InferedType(ps, a.t));
  }
}

action(typeOfLet, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Maybe(Scheme) sc = t_find(e->lhs->ident, as);
    if (!sc.none) {
      A_RUN(freshInst(sc.value), qt);
      A_RUN(typeOf(as, e->rhs), a);
      A_RUN(unify(qt.t, a.t));
      List(Pred) ps = appendPreds(qt.ps, a.ps);
      A_RETURN(InferedType(ps, a.t));
    } else {
      if (e->rhs->id == LAMBDA) {
        TypeT T = trait(Type);
        A_RUN(newTVar(trait(Kind).Star()), a);
        A_RUN(newTVar(trait(Kind).Star()), b);
        A_RUN(newTVar(trait(Kind).Star()), f);
        A_RUN(unify(T.func(a, b), f));
        Scheme sc = toScheme(f); // TODO !?
        as = t_add(e->lhs->ident, sc, as);
      }
      A_RUN(typeOf(as, e->rhs), x);
      A_RETURN(x);
    }
  }
}

action(typeOfOrAnd, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(typeOf(as, e->lhs), a);
    A_RUN(typeOf(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    A_RUN(unify(Bool, b.t));
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(Bool, t));
    A_RETURN(InferedType(NULL, t));
  }
}

action(typeOfComparisson, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(typeOf(as, e->lhs), a);
    A_RUN(typeOf(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(Bool, t));
    A_RETURN(InferedType(NULL, t));
  }
}

action(typeOfArithmetic, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Int = trait(Type).tcon_int();
    A_RUN(typeOf(as, e->lhs), a);
    A_RUN(typeOf(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    A_RUN(unify(Int, b.t));
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(Int, t));
    A_RETURN(InferedType(NULL, t));
  }
}

action(typeOfNeg, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Int = trait(Type).tcon_int();
    A_RUN(typeOf(as, e->rhs), t);
    A_RUN(unify(Int, t.t));
    A_RETURN(t);
  }
}

action(typeOfNot, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    Type Int = trait(Type).tcon_int();
    A_RUN(typeOf(as, e->rhs), a);
    A_RUN(getSubst(), sub);
    Type b = t_apply_subst(sub, a.t);
    Eq(Type) E = trait(Eq(Type));
    if (E.eq(b, Int) || E.eq(b, Bool)) {
      A_RETURN(a);
    }
    A_FAIL((TypeError){"Type mismatch"});
  }
}

action(typeOfLiteral, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    switch (e->literal.id) {
    case LIT_INTEGER:
      A_RETURN(InferedType(NULL, T.tcon_int()));
    case LIT_UNIT:
      A_RETURN(InferedType(NULL, T.tcon_unit()));
    case LIT_TRUE:
    case LIT_FALSE:
      A_RETURN(InferedType(NULL, T.tcon_bool()));
    default:
      A_FAIL((TypeError){"Illegal Literal"});
    }
  }
}

action(typeOfPrint, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    Type Unit = trait(Type).tcon_unit();
    A_RUN(typeOf(as, e->rhs));
    A_RETURN(InferedType(NULL, Unit));
  }
}

action(typeOfCon, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    ExprT E = trait(Expr);
    A_RUN(typeOf(as, E.var(e->ident)), t);
    A_RETURN(t);
  }
}

action(typeOfCapply, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    A_RUN(typeOf(as, e->lhs), xe);
    A_RUN(typeOf(as, e->rhs), xf);
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(T.func(xf.t, t), xe.t));
    List(Pred) ps = appendPreds(xe.ps, xf.ps);
    A_RETURN(InferedType(ps, t));
  }
}

action(typeOfFail, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    A_FAIL((TypeError){"Invalid expr"});
  }
}

static ACTION(Infered(Type)) typeOf0Impl(List(Assump) as, Expr e) {
  switch (e->id) {
  case VAR:
    return typeOfVar(as, e);
  case LAMBDA:
    return typeOfLambda(as, e);
  case APPLY:
    return typeOfApply(as, e);
  case IFELSE:
    return typeOfIfelse(as, e);
  case BLK:
    return typeOfBlk(as, e);
  case SEQ:
    return typeOfSeq(as, e);
  case DECLVAR:
    return typeOfDeclvar(as, e);
  case ASSIGN:
    return typeOfAssign(as, e);
  case LET:
    return typeOfLet(as, e);
  case OR:
  case AND:
    return typeOfOrAnd(as, e);
  case EQ:
  case NEQ:
  case LE:
  case LT:
  case GT:
  case GE:
    return typeOfComparisson(as, e);
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD:
    return typeOfArithmetic(as, e);
  case NEG:
    return typeOfNeg(as, e);
  case NOT:
    return typeOfNot(as, e);
  case LITERAL:
    return typeOfLiteral(as, e);
  case PRINT:
    return typeOfPrint(as, e);
  case CON:
    return typeOfCon(as, e);
  case CAPPLY:
    return typeOfCapply(as, e);
  default:
    return typeOfFail(as, e);
  }
}

static TI(Infered(Type)) typeOf0(List(Assump) as, Expr e) {
  return ti_label(typeOf0Impl(as, e), e);
}

// -----------------------------------------------------------------------
action(typeOf, List(Assump), Expr, Infered(Type)) {
  A_DO_WITH(as, e) {
    A_RUN(typeOf0(as, e), a);
    A_RUN(getSubst(), sub);
    a.t = t_apply_subst(sub, a.t);
    A_RETURN(a);
  }
}
