/* -*- coding: utf-8-unix -*- */

#include "types/TypeInferCombinator.h"

// -----------------------------------------------------------------------
fn(getSubstImpl, UnTypeInferArgs(TypeSubst)) {
  g_bind((s, ok), *args);
  return fn_apply(ok, s.subst, s);
}

TypeInfer(TypeSubst) getSubst(void) {
  return (TypeInfer(TypeSubst)){getSubstImpl()};
}

// -----------------------------------------------------------------------
typedef_Fn_r(Type, Type, UnTypeInfer(None));
fn(unifyImpl, Type, Type, UnTypeInferArgs(None)) {
  g_bind((t1, t2, s, ok, err), *args);
  Maybe(TypeSubst) u =
      t_unifier(t_apply_subst(s.subst, t1), t_apply_subst(s.subst, t2));
  if (u.none) {
    Show(Type) S = trait(Show(Type));
    CharBuff b = {0};
    mem_printf(&b, "Type mismatch\n  t1 = ");
    S.toString(&b, t1);
    mem_printf(&b, "\n  t2 = ");
    S.toString(&b, t2);
    TypeError e = {b.data};
    // TypeError e = {"Type mismatch"};
    return fn_apply(err, e, s);
  }
  s.subst = t_composite_subst(u.value, s.subst);
  return fn_apply(ok, (None){0}, s);
}

TypeInfer(None) unify(Type t1, Type t2) {
  __auto_type f = unifyImpl();
  return (TypeInfer(None)){fn_apply(f, t1, t2)};
}

// -----------------------------------------------------------------------
fn(newTVarImpl, UnTypeInferArgs(Type)) {
  g_bind((s, ok, err), *args);
  CharBuff b = {0};
  mem_printf(&b, "#a%d", s.i++);
  Type t = trait(Type).tvar((TVar){b.data});
  return fn_apply(ok, t, s);
}

TypeInfer(Type) newTVar(void) {
  return (TypeInfer(Type)){newTVarImpl()};
}

// -----------------------------------------------------------------------
static Type inst(Type* ts, size_t n, Type t) {
  switch (t->kind) {
  case TAPPLY:
    return trait(Type).tapply(inst(ts, n, t->lhs), inst(ts, n, t->rhs));
  case TANY:
    return ts[t->any.n];
  default:
    return t;
  }
}

typedef_Fn_r(TypeScheme, UnTypeInfer(Type));
fn(freshInstImpl, TypeScheme, UnTypeInferArgs(Type)) {
  g_bind((sc, s, ok, err), *args);
  if (!sc.numAny) {
    return fn_apply(ok, sc.type, s);
  }
  Type ts[sc.numAny];
  TypeInfer(Type) ti = newTVar();
  for (int i = 0; i < sc.numAny; i++) {
    TIResult(Type) r = runTypeInferP(ti, s);
    s = r.state;
    ts[i] = r.ok;
  }
  Type t = inst(ts, sc.numAny, sc.type);
  return fn_apply(ok, t, s);
}

TypeInfer(Type) freshInst(TypeScheme sc) {
  __auto_type f = freshInstImpl();
  return (TypeInfer(Type)){fn_apply(f, sc)};
}

// -----------------------------------------------------------------------
#define TI_UNIQUE_ID_NEW() TI_UNIQUE_ID_NEW0(__COUNTER__)
#define TI_UNIQUE_ID_NEW0(...) CAT(cparsec_unique_identifier, __VA_ARGS__)

#define TI_RUN(...)                                                      \
  CAT(TI_RUN, VARIADIC_SIZE(__VA_ARGS__))(TI_UNIQUE_ID_NEW(), __VA_ARGS__)

#define TI_RUN1(_id_, _ti_)                                              \
  __auto_type _id_ = runTypeInferP(_ti_, s);                             \
  s = _id_.state;                                                        \
  do {                                                                   \
    if (!_id_.success) {                                                 \
      return fn_apply(err, _id_.err, s);                                 \
    }                                                                    \
  } while (0)

#define TI_RUN2(_id_, _ti_, _x_)                                         \
  TI_RUN1(_id_, _ti_);                                                   \
  __auto_type _x_ = _id_.ok;

#define TI_RETURN(_x_) return fn_apply(ok, _x_, s)

#define TI_FAIL(_e_) return fn_apply(err, _e_, s)

// -----------------------------------------------------------------------
typedef_Fn_r(TypeInfer(None), Expr, UnTypeInfer(None));
fn(ti_labelImpl, TypeInfer(None), Expr, UnTypeInferArgs(None)) {
  g_bind((ti, e, s, ok, err), *args);
  TIResult(None) r = runTypeInferP(ti, s);
  if (r.success) {
    return fn_apply(ok, r.ok, r.state);
  }
  CharBuff b = {0};
  mem_printf(&b, "%s\n | %s", r.err.msg, trait(Show(Expr)).show(e));
  r.err.msg = b.data;
  return fn_apply(err, r.err, r.state);
}

TypeInfer(None) ti_label(TypeInfer(None) ti, Expr e) {
  switch (e->kind) {
  case SEQ:
  case BLK:
    return ti;
  default: {
    __auto_type f = ti_labelImpl();
    return (TypeInfer(None)){fn_apply(f, ti, e)};
  }
  }
}

// -----------------------------------------------------------------------
static TypeInfer(None) typeOf0(List(TypeAssumption) as, Expr e, Type t);

typedef_Fn_r(List(TypeAssumption), Expr, Fn(Type, UnTypeInfer(None)));
fn(typeOf0Impl, List(TypeAssumption), Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TypeT T = trait(Type);
  Type Unit = T.tcon_unit();
  Type Bool = T.tcon_bool();
  Type Int = T.tcon_int();
  switch (e->kind) {
  case VAR: {
    Maybe(TypeScheme) sc = t_find(e->var, as);
    if (sc.none) {
      CharBuff b = {0};
      mem_printf(&b, "Undefined variable - %s", e->var.ident);
      TI_FAIL((TypeError){b.data});
    }
    TI_RUN(freshInst(sc.value), t2);
    TI_RUN(unify(t2, t), x);
    TI_RETURN(x);
  }
  case LAMBDA: {
    TI_RUN(newTVar(), a);
    TI_RUN(newTVar(), b);
    TI_RUN(unify(T.funcType(a, b), t));
    TypeAssumption c = {.var = e->lhs->var, .scheme = {0, a}};
    as = trait(List(TypeAssumption)).cons(c, as);
    TI_RUN(typeOf0(as, e->rhs, b), x);
    TI_RETURN(x);
  }
  case APPLY: {
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->lhs, T.funcType(a, t)));
    TI_RUN(typeOf0(as, e->rhs, a), x);
    TI_RETURN(x);
  }
  case IFELSE: {
    TI_RUN(typeOf0(as, e->lhs, Bool));
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->rhs->lhs, a));
    TI_RUN(typeOf0(as, e->rhs->rhs, a));
    TI_RUN(getSubst(), sub);
    TI_RUN(unify(t_apply_subst(sub, a), t), x);
    TI_RETURN(x);
  }
  case BLK: {
    TI_RUN(typeOf0(as, e->rhs, t), x);
    TI_RETURN(x);
  }
  case SEQ: {
    switch (e->lhs->kind) {
    case DECLVAR: {
      TypeScheme sc = t_gen(as, e->lhs->rhs->texpr);
      TypeAssumption c = {.var = e->lhs->lhs->var, .scheme = sc};
      as = trait(List(TypeAssumption)).cons(c, as);
      TI_RUN(typeOf0(as, e->rhs, t), x);
      TI_RETURN(x);
    }
    case LET: {
      // TODO what should we do if the variable was already declared?
      if (e->lhs->rhs->kind == LAMBDA) {
        TI_RUN(newTVar(), a);
        TI_RUN(newTVar(), b);
        TI_RUN(newTVar(), f);
        TI_RUN(unify(T.funcType(a, b), f));
        TypeAssumption c = {.var = e->lhs->lhs->var, .scheme = {0, f}};
        as = trait(List(TypeAssumption)).cons(c, as);
      }
      TI_RUN(newTVar(), a);
      TI_RUN(typeOf0(as, e->lhs->rhs, a));
      TI_RUN(getSubst(), sub);
      TypeScheme sc = t_gen(as, t_apply_subst(sub, a));
      TypeAssumption c = {.var = e->lhs->lhs->var, .scheme = sc};
      as = trait(List(TypeAssumption)).cons(c, as);
      TI_RUN(typeOf0(as, e->rhs, t), x);
      TI_RETURN(x);
    }
    default: {
      TI_RUN(newTVar(), a);
      TI_RUN(typeOf0(as, e->lhs, a));
      TI_RUN(typeOf0(as, e->rhs, t), x);
      TI_RETURN(x);
    }
    }
  }
  case DECLVAR: {
    // TODO what should we do here?
    TI_RUN(unify(e->rhs->texpr, t), x);
    TI_RETURN(x);
  }
  case ASSIGN: {
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->lhs, a));
    TI_RUN(typeOf0(as, e->rhs, a));
    TI_RUN(getSubst(), sub);
    TI_RUN(unify(t_apply_subst(sub, a), t), x);
    TI_RETURN(x);
  }
  case LET: {
    // TODO what should we do here?
    Maybe(TypeScheme) sc = t_find(e->lhs->var, as);
    if (!sc.none) {
      TI_RUN(freshInst(sc.value), t2);
      TI_RUN(typeOf0(as, e->rhs, t2));
      TI_RUN(unify(t2, t), x);
      TI_RETURN(x);
    } else {
      if (e->rhs->kind == LAMBDA) {
        TI_RUN(newTVar(), a);
        TI_RUN(newTVar(), b);
        TI_RUN(newTVar(), f);
        TI_RUN(unify(T.funcType(a, b), f));
        TypeAssumption c = {.var = e->lhs->var, .scheme = {0, f}};
        as = trait(List(TypeAssumption)).cons(c, as);
      }
      TI_RUN(typeOf0(as, e->rhs, t), x);
      TI_RETURN(x);
    }
  }
  case OR:
  case AND: {
    TI_RUN(typeOf0(as, e->lhs, Bool));
    TI_RUN(typeOf0(as, e->rhs, Bool));
    TI_RUN(unify(Bool, t), x);
    TI_RETURN(x);
  }
  case EQ:
  case NEQ:
  case LE:
  case LT:
  case GT:
  case GE: {
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->lhs, a));
    TI_RUN(typeOf0(as, e->rhs, a));
    TI_RUN(unify(Bool, t), x);
    TI_RETURN(x);
  }
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD: {
    TI_RUN(typeOf0(as, e->lhs, Int));
    TI_RUN(typeOf0(as, e->rhs, Int));
    TI_RUN(unify(Int, t), x);
    TI_RETURN(x);
  }
  case NEG: {
    TI_RUN(typeOf0(as, e->rhs, Int));
    TI_RUN(unify(Int, t), x);
    TI_RETURN(x);
  }
  case NOT: {
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->rhs, a));
    TI_RUN(getSubst(), sub);
    Type b = t_apply_subst(sub, a);
    Eq(Type) E = trait(Eq(Type));
    if (E.eq(b, Int) || E.eq(b, Bool)) {
      TI_RUN(unify(b, t), x);
      TI_RETURN(x);
    }
    TI_FAIL((TypeError){"Type mismatch"});
  }
  case NUM: {
    TI_RUN(unify(Int, t), x);
    TI_RETURN(x);
  }
  case FALSE:
  case TRUE: {
    TI_RUN(unify(Bool, t), x);
    TI_RETURN(x);
  }
  case UNIT: {
    TI_RUN(unify(Unit, t), x);
    TI_RETURN(x);
  }
  case PRINT: {
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->rhs, a)); /* what we should do for `a`? */
    TI_RUN(unify(Unit, t), x);
    TI_RETURN(x);
  }
  default:
    TI_FAIL((TypeError){"Invalid expr"});
  }
}

static TypeInfer(None) typeOf0(List(TypeAssumption) as, Expr e, Type t) {
  __auto_type f = typeOf0Impl();
  // return (TypeInfer(None)){fn_apply(f, as, e, t)};
  TypeInfer(None) ti = (TypeInfer(None)){fn_apply(f, as, e, t)};
  return ti_label(ti, e);
}

// -----------------------------------------------------------------------
typedef_Fn_r(List(TypeAssumption), Expr, UnTypeInfer(Type));
fn(typeOfImpl, List(TypeAssumption), Expr, UnTypeInferArgs(Type)) {
  g_bind((as, e, s, ok, err), *args);
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e, a));
  TI_RUN(getSubst(), sub);
  TI_RETURN(t_apply_subst(sub, a));
}

TypeInfer(Type) typeOf(List(TypeAssumption) as, Expr e) {
  __auto_type f = typeOfImpl();
  return (TypeInfer(Type)){fn_apply(f, as, e)};
}
