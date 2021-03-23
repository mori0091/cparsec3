/* -*- coding: utf-8-unix -*- */

#include "types/Infer.h"

// -----------------------------------------------------------------------
// utility functions
// -----------------------------------------------------------------------
static inline Tup(List(Pred), Type) tupPsT(List(Pred) ps, Type t) {
  return (Tup(List(Pred), Type)){
      .ps = ps,
      .t = t,
  };
}

static inline Tup(List(Pred), List(Assump), Type)
    tupPsAsT(List(Pred) ps, List(Assump) as, Type t) {
  return (Tup(List(Pred), List(Assump), Type)){
      .ps = ps,
      .as = as,
      .t = t,
  };
}

static inline Tup(List(Pred), List(Assump), List(Type))
    tupPsAsTs(List(Pred) ps, List(Assump) as, List(Type) ts) {
  return (Tup(List(Pred), List(Assump), List(Type))){
      .ps = ps,
      .as = as,
      .ts = ts,
  };
}

#define impl_append(T)                                                   \
  static List(T) FUNC_NAME(append, List(T))(List(T) ps1, List(T) ps2) {  \
    if (!ps1) {                                                          \
      return ps2;                                                        \
    }                                                                    \
    if (!ps2) {                                                          \
      return ps1;                                                        \
    }                                                                    \
    List(T) xs = ps1;                                                    \
    while (xs->tail) {                                                   \
      xs = xs->tail;                                                     \
    }                                                                    \
    xs->tail = ps2;                                                      \
    return ps1;                                                          \
  }                                                                      \
  END_OF_STATEMENTS

impl_append(Pred);
static List(Pred) appendPreds(List(Pred) ps1, List(Pred) ps2) {
  return FUNC_NAME(append, List(Pred))(ps1, ps2);
}

impl_append(Assump);
static List(Assump) appendAssumps(List(Assump) ps1, List(Assump) ps2) {
  return FUNC_NAME(append, List(Assump))(ps1, ps2);
}

static Type foldrFn(Type t, List(Type) ts) {
  if (!ts) {
    return t;
  }
  return trait(Type).func(ts->head, foldrFn(t, ts->tail));
}

// -----------------------------------------------------------------------
// utility monad
// -----------------------------------------------------------------------
action(ti_label0, TI(Tup(List(Pred), Type)), Expr,
       Tup(List(Pred), Type)) {
  A_DO_WITH(ti, e) {
    A_RUN(getState(), s);
    TIResult(Tup(List(Pred), Type)) r = runTI(ti, s);
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

static TI(Tup(List(Pred), Type))
    ti_label(TI(Tup(List(Pred), Type)) ti, Expr e) {
  switch (e->id) {
  case SEQ:
  case BLK:
    return ti;
  default:
    return ti_label0(ti, e);
  }
}

// -----------------------------------------------------------------------
// high level type-inference monads
// -----------------------------------------------------------------------
action(tiProgram, ClassEnv, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(, as, e) { /* ClassEnv is unused yet */
    A_RUN(tiExpr(as, e), a);
    A_RUN(getSubst(), sub);
    a.t = t_apply_subst(sub, a.t);
    A_RETURN(a);
  }
}

action(fixPats, List(Assump), List(Pat), None) {
  A_DO_WITH(as, pats) {
    for (List(Pat) ps = pats; ps; ps = ps->tail) {
      Pat p = ps->head;
      if (p->id == PCON) {
        Maybe(Scheme) sc = t_find(p->a.ident, as);
        if (sc.none) {
          CharBuff b = {0};
          mem_printf(&b, "Undefined constructor - %s", p->a.ident);
          A_FAIL((TypeError){b.data});
        }
        A_RUN(fixPats(as, p->pats));
        p->a.scheme = sc.value;
      }
    }
    A_RETURN((None){0});
  }
}

action(tiAlt, ClassEnv, List(Assump), Alt, Tup(List(Pred), Type)) {
  A_DO_WITH(, as, alt) { /* ClassEnv is unused yet */
    /* check and correct each constructor pattern in `alt.pats` */
    A_RUN(fixPats(as, alt.pats));
    /* ---- */
    A_RUN(tiPats(alt.pats), psasts);
    A_RUN(tiExpr(appendAssumps(psasts.as, as), alt.e), pst);
    List(Pred) ps = appendPreds(psasts.ps, pst.ps);
    Type t = foldrFn(pst.t, psasts.ts);
    A_RETURN(tupPsT(ps, t));
  }
}

action(tiAlts, ClassEnv, List(Assump), List(Alt), Type, List(Pred)) {
  A_DO_WITH(ce, as, alts, t) {
    List(Pred) ps = NULL;
    while (alts) {
      A_RUN(tiAlt(ce, as, alts->head), pst);
      ps = appendPreds(ps, pst.ps);
      A_RUN(unify(t, pst.t));
      alts = alts->tail;
    }
    A_RETURN(ps);
  }
}

// -----------------------------------------------------------------------
// middle level type-inference monads
// -----------------------------------------------------------------------
static TI(Tup(List(Pred), Type)) tiExpr0(List(Assump) as, Expr e);

TI(Tup(List(Pred), Type)) tiExpr(List(Assump) as, Expr e) {
  return ti_label(tiExpr0(as, e), e);
}

action(tiLiteral, Literal, Tup(List(Pred), Type)) {
  A_DO_WITH(lit) {
    TypeT T = trait(Type);
    switch (lit.id) {
    case LIT_INTEGER:
      A_RETURN(tupPsT(NULL, T.tcon_int()));
    case LIT_UNIT:
      A_RETURN(tupPsT(NULL, T.tcon_unit()));
    case LIT_TRUE:
    case LIT_FALSE:
      A_RETURN(tupPsT(NULL, T.tcon_bool()));
    case LIT_STRING:
      A_RETURN(tupPsT(NULL, T.tcon_String()));
    default:
      A_FAIL((TypeError){"Illegal Literal"});
    }
  }
}

action(tiPat, Pat, Tup(List(Pred), List(Assump), Type)) {
  A_DO_WITH(pat) {
    KindT K = trait(Kind);
    Assumption A = trait(Assumption);
    switch (pat->id) {
    case PWILDCARD: {
      A_RUN(newTVar(K.Star()), v);
      A_RETURN(tupPsAsT(NULL, NULL, v));
    }
    case PVAR: {
      A_RUN(newTVar(K.Star()), v);
      List(Assump) as = A.add(pat->ident, toScheme(v), NULL);
      A_RETURN(tupPsAsT(NULL, as, v));
    }
    case PLITERAL: {
      A_RUN(tiLiteral(pat->literal), a);
      A_RETURN(tupPsAsT(a.ps, NULL, a.t));
    }
    case PCON: {
      A_RUN(tiPats(pat->pats), psasts);
      A_RUN(newTVar(K.Star()), t2);
      A_RUN(freshInst(pat->a.scheme), qt);
      A_RUN(unify(qt.t, foldrFn(t2, psasts.ts)));
      List(Pred) ps = appendPreds(psasts.ps, qt.ps);
      List(Assump) as = psasts.as;
      A_RETURN(tupPsAsT(ps, as, t2));
    }
    default:
      A_FAIL((TypeError){"Illegal Pat"});
    }
  }
}

action(tiPats, List(Pat), Tup(List(Pred), List(Assump), List(Type))) {
  A_DO_WITH(pats) {
    List(Pred) ps = NULL;
    List(Assump) as = NULL;
    List(Type) ts = NULL;
    while (pats) {
      A_RUN(tiPat(pats->head), psast);
      ps = appendPreds(ps, psast.ps);
      as = appendAssumps(as, psast.as);
      ts = trait(List(Type)).cons(psast.t, ts);
      pats = pats->tail;
    }
    trait(List(Type)).reverse(&ts);
    A_RETURN(tupPsAsTs(ps, as, ts));
  }
}

// -----------------------------------------------------------------------
action(tiExprVar, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Maybe(Scheme) sc = t_find(e->ident, as);
    if (sc.none) {
      CharBuff b = {0};
      mem_printf(&b, "Undefined variable - %s", e->ident);
      A_FAIL((TypeError){b.data});
    }
    A_RUN(freshInst(sc.value), qt);
    A_RETURN(tupPsT(qt.ps, qt.t));
  }
}

action(tiExprLambda, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    A_RUN(newTVar(trait(Kind).Star()), a);
    A_RUN(newTVar(trait(Kind).Star()), b);
    A_RUN(newTVar(trait(Kind).Star()), f);
    A_RUN(unify(T.func(a, b), f));
    Scheme sc = toScheme(a); // TODO !?
    as = t_add(e->lhs->ident, sc, as);
    A_RUN(tiExpr(as, e->rhs), c);
    A_RUN(unify(c.t, b));
    A_RETURN(tupPsT(NULL, f));
  }
}

action(tiExprApply, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    A_RUN(tiExpr(as, e->lhs), a);
    A_RUN(tiExpr(as, e->rhs), b);
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(T.func(b.t, t), a.t));
    List(Pred) ps = appendPreds(a.ps, b.ps);
    A_RETURN(tupPsT(ps, t));
  }
}

action(tiExprMatch, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    ClassEnv ce = initialEnv(); /* not used yet */
    A_RUN(newTVar(trait(Kind).Star()), f);
    A_RUN(tiAlts(ce, as, e->alts, f), ps);
    TypeT T = trait(Type);
    A_RUN(tiExpr(as, e->match_arg), pst);
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(T.func(pst.t, t), f));
    A_RETURN(tupPsT(appendPreds(pst.ps, ps), t));
  }
}

action(tiExprIfelse, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(tiExpr(as, e->lhs), c);
    A_RUN(unify(c.t, Bool));
    A_RUN(tiExpr(as, e->rhs->lhs), a);
    A_RUN(tiExpr(as, e->rhs->rhs), b);
    A_RUN(unify(a.t, b.t));
    List(Pred) ps = appendPreds(a.ps, b.ps);
    A_RETURN(tupPsT(ps, a.t));
  }
}

action(tiExprBlk, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    A_RUN(tiExpr(as, e->rhs), x);
    A_RETURN(x);
  }
}

action(tiExprSeq, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    TypeT T = trait(Type);
    switch (e->lhs->id) {
    case DECLVAR: {
      Scheme sc = t_gen(as, e->lhs->rhs->texpr);
      as = t_add(e->lhs->lhs->ident, sc, as);
      A_RUN(tiExpr(as, e->rhs), x);
      A_RETURN(x);
    }
    case LET: {
      Maybe(Scheme) m = t_find(e->lhs->lhs->ident, as);
      if (m.none) {
        if (e->lhs->rhs->id == LAMBDA) {
          A_RUN(newTVar(trait(Kind).Star()), a);
          A_RUN(newTVar(trait(Kind).Star()), b);
          A_RUN(newTVar(trait(Kind).Star()), f);
          A_RUN(unify(T.func(a, b), f));
          Scheme sc = toScheme(f); // TODO !?
          as = t_add(e->lhs->lhs->ident, sc, as);
        }
      }
      A_RUN(tiExpr(as, e->lhs->rhs), a);
      A_RUN(getSubst(), sub);
      Scheme sc = t_gen(as, t_apply_subst(sub, a.t));
      as = t_add(e->lhs->lhs->ident, sc, as);
      A_RUN(tiExpr(as, e->rhs), x);
      List(Pred) ps = appendPreds(a.ps, x.ps);
      A_RETURN(tupPsT(ps, x.t));
    }
    default: {
      A_RUN(tiExpr(as, e->lhs));
      A_RUN(tiExpr(as, e->rhs), t);
      A_RETURN(t);
    }
    }
  }
}

action(tiExprDeclvar, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    // TODO what should we do here?
    A_RETURN(tupPsT(NULL, e->rhs->texpr));
  }
}

action(tiExprLet, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Maybe(Scheme) sc = t_find(e->lhs->ident, as);
    if (!sc.none) {
      A_RUN(freshInst(sc.value), qt);
      A_RUN(tiExpr(as, e->rhs), a);
      A_RUN(unify(qt.t, a.t));
      List(Pred) ps = appendPreds(qt.ps, a.ps);
      A_RETURN(tupPsT(ps, a.t));
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
      A_RUN(tiExpr(as, e->rhs), x);
      A_RETURN(x);
    }
  }
}

action(tiExprOrAnd, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(tiExpr(as, e->lhs), a);
    A_RUN(tiExpr(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    A_RUN(unify(Bool, b.t));
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(Bool, t));
    A_RETURN(tupPsT(NULL, t));
  }
}

action(tiExprComparisson, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(tiExpr(as, e->lhs), a);
    A_RUN(tiExpr(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(Bool, t));
    A_RETURN(tupPsT(NULL, t));
  }
}

action(tiExprArithmetic, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Int = trait(Type).tcon_int();
    A_RUN(tiExpr(as, e->lhs), a);
    A_RUN(tiExpr(as, e->rhs), b);
    A_RUN(unify(a.t, b.t));
    A_RUN(unify(Int, b.t));
    A_RUN(newTVar(trait(Kind).Star()), t);
    A_RUN(unify(Int, t));
    A_RETURN(tupPsT(NULL, t));
  }
}

action(tiExprNeg, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Int = trait(Type).tcon_int();
    A_RUN(tiExpr(as, e->rhs), t);
    A_RUN(unify(Int, t.t));
    A_RETURN(t);
  }
}

action(tiExprComplement, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Bool = trait(Type).tcon_bool();
    Type Int = trait(Type).tcon_int();
    A_RUN(tiExpr(as, e->rhs), a);
    A_RUN(getState(), st);
    __auto_type r = runTI(unify(Bool, a.t), st);
    if (!r.success) {
      A_RUN(unify(Int, a.t));
    }
    A_RETURN(a);
  }
}

action(tiExprPrint, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    Type Unit = trait(Type).tcon_unit();
    A_RUN(tiExpr(as, e->rhs));
    A_RETURN(tupPsT(NULL, Unit));
  }
}

action(tiExprCon, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    ExprT E = trait(Expr);
    Expr x = E.var(e->ident);
    for (List(Expr) es = e->args; es; es = es->tail) {
      x = E.apply(x, es->head);
    }
    A_RUN(tiExpr(as, x), t);
    A_RETURN(t);
  }
}

action(tiExprFail, List(Assump), Expr, Tup(List(Pred), Type)) {
  A_DO_WITH(as, e) {
    A_FAIL((TypeError){"Invalid expr"});
  }
}

static ACTION(Tup(List(Pred), Type)) tiExpr0(List(Assump) as, Expr e) {
  switch (e->id) {
  case VAR:
    return tiExprVar(as, e);
  case LAMBDA:
    return tiExprLambda(as, e);
  case APPLY:
    return tiExprApply(as, e);
  case MATCH:
    return tiExprMatch(as, e);
  case IFELSE:
    return tiExprIfelse(as, e);
  case BLK:
    return tiExprBlk(as, e);
  case SEQ:
    return tiExprSeq(as, e);
  case DECLVAR:
    return tiExprDeclvar(as, e);
  case LET:
    return tiExprLet(as, e);
  case OR:
  case AND:
    return tiExprOrAnd(as, e);
  case EQ:
  case NEQ:
  case LE:
  case LT:
  case GT:
  case GE:
    return tiExprComparisson(as, e);
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD:
    return tiExprArithmetic(as, e);
  case NEG:
    return tiExprNeg(as, e);
  case COMPLEMENT:
    return tiExprComplement(as, e);
  case LITERAL:
    return tiLiteral(e->literal);
  case PRINT:
    return tiExprPrint(as, e);
  case CON:
    return tiExprCon(as, e);
  case CCON:
    return tiExprCon(as, e->con);
  default:
    return tiExprFail(as, e);
  }
}
