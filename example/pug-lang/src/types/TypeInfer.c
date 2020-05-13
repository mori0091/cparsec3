/* -*- coding: utf-8-unix -*- */

#include "types/TypeInfer.h"

// -----------------------------------------------------------------------
action(getSubst, Subst) {
  A_DO() {
    A_RUN(getState(), s);
    A_RETURN(s.subst);
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
    s.subst = t_composite_subst(u.value, s.subst);
    A_RUN(putState(s));
    A_RETURN((None){0});
  }
}

// -----------------------------------------------------------------------
action(newTVar, Type) {
  A_DO() {
    A_RUN(getState(), s);
    CharBuff b = {0};
    mem_printf(&b, "#a%d", s.i++);
    Type t = trait(Type).TVar(b.data, trait(Kind).Star());
    A_RUN(putState(s));
    A_RETURN(t);
  }
}

// -----------------------------------------------------------------------
static Type inst(Type* ts, size_t n, Type t) {
  switch (t->id) {
  case TAPPLY:
    return trait(Type).TAp(inst(ts, n, t->lhs), inst(ts, n, t->rhs));
  case TGEN:
    return ts[t->tgen.n];
  default:
    return t;
  }
}

action(freshInst, Scheme, Type) {
  A_DO_WITH(sc) {
    if (!sc.numTGen) {
      A_RETURN(sc.type);
    }
    Type ts[sc.numTGen];
    ACTION(Type) ti = newTVar();
    for (int i = 0; i < sc.numTGen; i++) {
      A_RUN(ti, x);
      ts[i] = x;
    }
    Type t = inst(ts, sc.numTGen, sc.type);
    A_RETURN(t);
  }
}

// -----------------------------------------------------------------------
action(ti_label0, ACTION(None), Expr, None) {
  A_DO_WITH(ti, e) {
    A_RUN(getState(), s);
    A_RESULT(None) r = runAction(ti, s);
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

ACTION(None) ti_label(ACTION(None) ti, Expr e) {
  switch (e->id) {
  case SEQ:
  case BLK:
    return ti;
  default:
    return ti_label0(ti, e);
  }
}

// -----------------------------------------------------------------------
static ACTION(None) typeOf0(List(Assump) as, Expr e, Type t);

action(typeOfVar, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Maybe(Scheme) sc = t_find(e->var, as);
    if (sc.none) {
      CharBuff b = {0};
      mem_printf(&b, "Undefined variable - %s", e->var.ident);
      A_FAIL((TypeError){b.data});
    }
    A_RUN(freshInst(sc.value), t2);
    A_RUN(unify(t2, t), x);
    A_RETURN(x);
  }
}

action(typeOfLambda, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    TypeT T = trait(Type);
    A_RUN(newTVar(), a);
    A_RUN(newTVar(), b);
    A_RUN(unify(T.func(a, b), t));
    Scheme sc = {0, a};
    as = t_add(e->lhs->var, sc, as);
    A_RUN(typeOf0(as, e->rhs, b), x);
    A_RETURN(x);
  }
}

action(typeOfApply, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    TypeT T = trait(Type);
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e->lhs, T.func(a, t)));
    A_RUN(typeOf0(as, e->rhs, a), x);
    A_RETURN(x);
  }
}

action(typeOfIfelse, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(typeOf0(as, e->lhs, Bool));
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e->rhs->lhs, a));
    A_RUN(typeOf0(as, e->rhs->rhs, a));
    A_RUN(getSubst(), sub);
    A_RUN(unify(t_apply_subst(sub, a), t), x);
    A_RETURN(x);
  }
}

action(typeOfBlk, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    A_RUN(typeOf0(as, e->rhs, t), x);
    A_RETURN(x);
  }
}

action(typeOfSeq, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    TypeT T = trait(Type);
    switch (e->lhs->id) {
    case DECLVAR: {
      Scheme sc = t_gen(as, e->lhs->rhs->texpr);
      as = t_add(e->lhs->lhs->var, sc, as);
      A_RUN(typeOf0(as, e->rhs, t), x);
      A_RETURN(x);
    }
    case LET: {
      Maybe(Scheme) sc = t_find(e->lhs->var, as);
      if (!sc.none) {
        A_RUN(freshInst(sc.value), a);
        A_RUN(typeOf0(as, e->lhs->rhs, a));
        A_RUN(getSubst(), sub);
        Scheme sc = t_gen(as, t_apply_subst(sub, a));
        as = t_add(e->lhs->lhs->var, sc, as);
        A_RUN(typeOf0(as, e->rhs, t), x);
        A_RETURN(x);
      } else {
        if (e->lhs->rhs->id == LAMBDA) {
          A_RUN(newTVar(), a);
          A_RUN(newTVar(), b);
          A_RUN(newTVar(), f);
          A_RUN(unify(T.func(a, b), f));
          Scheme sc = {0, f};
          as = t_add(e->lhs->lhs->var, sc, as);
        }
        A_RUN(newTVar(), a);
        A_RUN(typeOf0(as, e->lhs->rhs, a));
        A_RUN(getSubst(), sub);
        Scheme sc = t_gen(as, t_apply_subst(sub, a));
        as = t_add(e->lhs->lhs->var, sc, as);
        A_RUN(typeOf0(as, e->rhs, t), x);
        A_RETURN(x);
      }
    }
    default: {
      A_RUN(newTVar(), a);
      A_RUN(typeOf0(as, e->lhs, a));
      A_RUN(typeOf0(as, e->rhs, t), x);
      A_RETURN(x);
    }
    }
  }
}

action(typeOfDeclvar, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    // TODO what should we do here?
    A_RUN(unify(e->rhs->texpr, t), x);
    A_RETURN(x);
  }
}

action(typeOfAssign, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e->lhs, a));
    A_RUN(typeOf0(as, e->rhs, a));
    A_RUN(getSubst(), sub);
    A_RUN(unify(t_apply_subst(sub, a), t), x);
    A_RETURN(x);
  }
}

action(typeOfLet, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Maybe(Scheme) sc = t_find(e->lhs->var, as);
    if (!sc.none) {
      A_RUN(freshInst(sc.value), a);
      A_RUN(typeOf0(as, e->rhs, a));
      A_RUN(unify(a, t), x);
      A_RETURN(x);
    } else {
      if (e->rhs->id == LAMBDA) {
        TypeT T = trait(Type);
        A_RUN(newTVar(), a);
        A_RUN(newTVar(), b);
        A_RUN(newTVar(), f);
        A_RUN(unify(T.func(a, b), f));
        Scheme sc = {0, f};
        as = t_add(e->lhs->var, sc, as);
      }
      A_RUN(typeOf0(as, e->rhs, t), x);
      A_RETURN(x);
    }
  }
}

action(typeOfOrAnd, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(typeOf0(as, e->lhs, Bool));
    A_RUN(typeOf0(as, e->rhs, Bool));
    A_RUN(unify(Bool, t), x);
    A_RETURN(x);
  }
}

action(typeOfComparisson, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Bool = trait(Type).tcon_bool();
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e->lhs, a));
    A_RUN(typeOf0(as, e->rhs, a));
    A_RUN(unify(Bool, t), x);
    A_RETURN(x);
  }
}

action(typeOfArithmetic, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Int = trait(Type).tcon_int();
    A_RUN(typeOf0(as, e->lhs, Int));
    A_RUN(typeOf0(as, e->rhs, Int));
    A_RUN(unify(Int, t), x);
    A_RETURN(x);
  }
}

action(typeOfNeg, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Int = trait(Type).tcon_int();
    A_RUN(typeOf0(as, e->rhs, Int));
    A_RUN(unify(Int, t), x);
    A_RETURN(x);
  }
}

action(typeOfNot, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Bool = trait(Type).tcon_bool();
    Type Int = trait(Type).tcon_int();
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e->rhs, a));
    A_RUN(getSubst(), sub);
    Type b = t_apply_subst(sub, a);
    Eq(Type) E = trait(Eq(Type));
    if (E.eq(b, Int) || E.eq(b, Bool)) {
      A_RUN(unify(b, t), x);
      A_RETURN(x);
    }
    A_FAIL((TypeError){"Type mismatch"});
  }
}

action(typeOfNum, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Int = trait(Type).tcon_int();
    A_RUN(unify(Int, t), x);
    A_RETURN(x);
  }
}

action(typeOfFalseTrue, List(Assump), Expr, Type, None) {
  Type Bool = trait(Type).tcon_bool();
  A_DO_WITH(as, e, t) {
    A_RUN(unify(Bool, t), x);
    A_RETURN(x);
  }
}

action(typeOfUnit, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Unit = trait(Type).tcon_unit();
    A_RUN(unify(Unit, t), x);
    A_RETURN(x);
  }
}

action(typeOfPrint, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    Type Unit = trait(Type).tcon_unit();
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e->rhs, a)); /* what we should do for `a`? */
    A_RUN(unify(Unit, t), x);
    A_RETURN(x);
  }
}

action(typeOfCon, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    ExprT E = trait(Expr);
    A_RUN(typeOf0(as, E.var((Var){e->con.ident}), t), x);
    A_RETURN(x);
  }
}

action(typeOfCapply, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    // TODO fix this ; does not work expected.
    TypeT T = trait(Type);
    A_RUN(newTVar(), a);
    A_RUN(newTVar(), b);
    A_RUN(typeOf0(as, e->rhs, a));
    A_RUN(typeOf0(as, e->lhs, T.func(a, b)));
    A_RUN(unify(b, t), x);
    A_RETURN(x);

    // NOTE the below works as expecetd,
    // if and only if `e` was `Just x` of `Maybe a` type.
    // ~~~c
    // TypeT T = trait(Type);
    // KindT K = trait(Kind);
    // A_RUN(newTVar(), a);
    // Type b = T.TAp(T.TCon("Maybe", K.Star()), a);
    // A_RUN(typeOf0(as, e->rhs, a));
    // A_RUN(typeOf0(as, e->lhs, T.func(a, b)));
    // A_RUN(unify(b, t), x);
    // A_RETURN(x);
    // ~~~
  }
}

action(typeOfFail, List(Assump), Expr, Type, None) {
  A_DO_WITH(as, e, t) {
    A_FAIL((TypeError){"Invalid expr"});
  }
}

static ACTION(None) typeOf0Impl(List(Assump) as, Expr e, Type t) {
  switch (e->id) {
  case VAR:
    return typeOfVar(as, e, t);
  case LAMBDA:
    return typeOfLambda(as, e, t);
  case APPLY:
    return typeOfApply(as, e, t);
  case IFELSE:
    return typeOfIfelse(as, e, t);
  case BLK:
    return typeOfBlk(as, e, t);
  case SEQ:
    return typeOfSeq(as, e, t);
  case DECLVAR:
    return typeOfDeclvar(as, e, t);
  case ASSIGN:
    return typeOfAssign(as, e, t);
  case LET:
    return typeOfLet(as, e, t);
  case OR:
  case AND:
    return typeOfOrAnd(as, e, t);
  case EQ:
  case NEQ:
  case LE:
  case LT:
  case GT:
  case GE:
    return typeOfComparisson(as, e, t);
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD:
    return typeOfArithmetic(as, e, t);
  case NEG:
    return typeOfNeg(as, e, t);
  case NOT:
    return typeOfNot(as, e, t);
  case NUM:
    return typeOfNum(as, e, t);
  case FALSE:
  case TRUE:
    return typeOfFalseTrue(as, e, t);
  case UNIT:
    return typeOfUnit(as, e, t);
  case PRINT:
    return typeOfPrint(as, e, t);
  case CON:
    return typeOfCon(as, e, t);
  case CAPPLY:
    return typeOfCapply(as, e, t);
  default:
    return typeOfFail(as, e, t);
  }
}

static TypeInfer(None) typeOf0(List(Assump) as, Expr e, Type t) {
  return ti_label(typeOf0Impl(as, e, t), e);
}

// -----------------------------------------------------------------------
action(typeOf, List(Assump), Expr, Type) {
  A_DO_WITH(as, e) {
    A_RUN(newTVar(), a);
    A_RUN(typeOf0(as, e, a));
    A_RUN(getSubst(), sub);
    A_RETURN(t_apply_subst(sub, a));
  }
}
