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
  Type t = trait(Type).tvar((Tyvar){b.data});
  return fn_apply(ok, t, s);
}

TypeInfer(Type) newTVar(void) {
  return (TypeInfer(Type)){newTVarImpl()};
}

// -----------------------------------------------------------------------
static Type inst(Type* ts, size_t n, Type t) {
  switch (t->id) {
  case TAPPLY:
    return trait(Type).tapply(inst(ts, n, t->lhs), inst(ts, n, t->rhs));
  case TGEN:
    return ts[t->tgen.n];
  default:
    return t;
  }
}

typedef_Fn_r(TypeScheme, UnTypeInfer(Type));
fn(freshInstImpl, TypeScheme, UnTypeInferArgs(Type)) {
  g_bind((sc, s, ok, err), *args);
  if (!sc.numTGen) {
    return fn_apply(ok, sc.type, s);
  }
  Type ts[sc.numTGen];
  TypeInfer(Type) ti = newTVar();
  for (int i = 0; i < sc.numTGen; i++) {
    TIResult(Type) r = runTypeInferP(ti, s);
    s = r.state;
    ts[i] = r.ok;
  }
  Type t = inst(ts, sc.numTGen, sc.type);
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
  switch (e->id) {
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
static TypeInfer(None) typeOf0(TAList as, Expr e, Type t);

typedef_Fn_r(TAList, Expr, Fn(Type, UnTypeInfer(None)));

fn(typeOfVar, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
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

fn(typeOfLambda, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TypeT T = trait(Type);
  TI_RUN(newTVar(), a);
  TI_RUN(newTVar(), b);
  TI_RUN(unify(T.func(a, b), t));
  TypeScheme sc = {0, a};
  as = t_add(e->lhs->var, sc, as);
  TI_RUN(typeOf0(as, e->rhs, b), x);
  TI_RETURN(x);
}

fn(typeOfApply, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TypeT T = trait(Type);
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e->lhs, T.func(a, t)));
  TI_RUN(typeOf0(as, e->rhs, a), x);
  TI_RETURN(x);
}

fn(typeOfIfelse, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Bool = trait(Type).tcon_bool();
  TI_RUN(typeOf0(as, e->lhs, Bool));
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e->rhs->lhs, a));
  TI_RUN(typeOf0(as, e->rhs->rhs, a));
  TI_RUN(getSubst(), sub);
  TI_RUN(unify(t_apply_subst(sub, a), t), x);
  TI_RETURN(x);
}

fn(typeOfBlk, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TI_RUN(typeOf0(as, e->rhs, t), x);
  TI_RETURN(x);
}

fn(typeOfSeq, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TypeT T = trait(Type);
  switch (e->lhs->id) {
  case DECLVAR: {
    TypeScheme sc = t_gen(as, e->lhs->rhs->texpr);
    as = t_add(e->lhs->lhs->var, sc, as);
    TI_RUN(typeOf0(as, e->rhs, t), x);
    TI_RETURN(x);
  }
  case LET: {
    Maybe(TypeScheme) sc = t_find(e->lhs->var, as);
    if (!sc.none) {
      TI_RUN(freshInst(sc.value), a);
      TI_RUN(typeOf0(as, e->lhs->rhs, a));
      TI_RUN(getSubst(), sub);
      TypeScheme sc = t_gen(as, t_apply_subst(sub, a));
      as = t_add(e->lhs->lhs->var, sc, as);
      TI_RUN(typeOf0(as, e->rhs, t), x);
      TI_RETURN(x);
    } else {
      if (e->lhs->rhs->id == LAMBDA) {
        TI_RUN(newTVar(), a);
        TI_RUN(newTVar(), b);
        TI_RUN(newTVar(), f);
        TI_RUN(unify(T.func(a, b), f));
        TypeScheme sc = {0, f};
        as = t_add(e->lhs->lhs->var, sc, as);
      }
      TI_RUN(newTVar(), a);
      TI_RUN(typeOf0(as, e->lhs->rhs, a));
      TI_RUN(getSubst(), sub);
      TypeScheme sc = t_gen(as, t_apply_subst(sub, a));
      as = t_add(e->lhs->lhs->var, sc, as);
      TI_RUN(typeOf0(as, e->rhs, t), x);
      TI_RETURN(x);
    }
  }
  default: {
    TI_RUN(newTVar(), a);
    TI_RUN(typeOf0(as, e->lhs, a));
    TI_RUN(typeOf0(as, e->rhs, t), x);
    TI_RETURN(x);
  }
  }
}

fn(typeOfDeclvar, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  // TODO what should we do here?
  TI_RUN(unify(e->rhs->texpr, t), x);
  TI_RETURN(x);
}

fn(typeOfAssign, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e->lhs, a));
  TI_RUN(typeOf0(as, e->rhs, a));
  TI_RUN(getSubst(), sub);
  TI_RUN(unify(t_apply_subst(sub, a), t), x);
  TI_RETURN(x);
}

fn(typeOfLet, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Maybe(TypeScheme) sc = t_find(e->lhs->var, as);
  if (!sc.none) {
    TI_RUN(freshInst(sc.value), a);
    TI_RUN(typeOf0(as, e->rhs, a));
    TI_RUN(unify(a, t), x);
    TI_RETURN(x);
  } else {
    if (e->rhs->id == LAMBDA) {
      TypeT T = trait(Type);
      TI_RUN(newTVar(), a);
      TI_RUN(newTVar(), b);
      TI_RUN(newTVar(), f);
      TI_RUN(unify(T.func(a, b), f));
      TypeScheme sc = {0, f};
      as = t_add(e->lhs->var, sc, as);
    }
    TI_RUN(typeOf0(as, e->rhs, t), x);
    TI_RETURN(x);
  }
}

fn(typeOfOrAnd, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Bool = trait(Type).tcon_bool();
  TI_RUN(typeOf0(as, e->lhs, Bool));
  TI_RUN(typeOf0(as, e->rhs, Bool));
  TI_RUN(unify(Bool, t), x);
  TI_RETURN(x);
}

fn(typeOfComparisson, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Bool = trait(Type).tcon_bool();
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e->lhs, a));
  TI_RUN(typeOf0(as, e->rhs, a));
  TI_RUN(unify(Bool, t), x);
  TI_RETURN(x);
}

fn(typeOfArithmetic, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Int = trait(Type).tcon_int();
  TI_RUN(typeOf0(as, e->lhs, Int));
  TI_RUN(typeOf0(as, e->rhs, Int));
  TI_RUN(unify(Int, t), x);
  TI_RETURN(x);
}

fn(typeOfNeg, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Int = trait(Type).tcon_int();
  TI_RUN(typeOf0(as, e->rhs, Int));
  TI_RUN(unify(Int, t), x);
  TI_RETURN(x);
}

fn(typeOfNot, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Bool = trait(Type).tcon_bool();
  Type Int = trait(Type).tcon_int();
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

fn(typeOfNum, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Int = trait(Type).tcon_int();
  TI_RUN(unify(Int, t), x);
  TI_RETURN(x);
}

fn(typeOfFalseTrue, TAList, Expr, Type, UnTypeInferArgs(None)) {
  Type Bool = trait(Type).tcon_bool();
  g_bind((as, e, t, s, ok, err), *args);
  TI_RUN(unify(Bool, t), x);
  TI_RETURN(x);
}

fn(typeOfUnit, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Unit = trait(Type).tcon_unit();
  TI_RUN(unify(Unit, t), x);
  TI_RETURN(x);
}

fn(typeOfPrint, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  Type Unit = trait(Type).tcon_unit();
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e->rhs, a)); /* what we should do for `a`? */
  TI_RUN(unify(Unit, t), x);
  TI_RETURN(x);
}

fn(typeOfCon, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  ExprT E = trait(Expr);
  TI_RUN(typeOf0(as, E.var((Var){e->con.ident}), t), x);
  TI_RETURN(x);
}

fn(typeOfCapply, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  // TODO fix this ; does not work expected.
  TypeT T = trait(Type);
  TI_RUN(newTVar(), a);
  TI_RUN(newTVar(), b);
  TI_RUN(typeOf0(as, e->rhs, a));
  TI_RUN(typeOf0(as, e->lhs, T.func(a, b)));
  TI_RUN(unify(b, t), x);
  TI_RETURN(x);

  // NOTE the below works as expecetd,
  // if and only if `e` was `Just x` of `Maybe a` type.
  // ~~~c
  // TypeT T = trait(Type);
  // TI_RUN(newTVar(), a);
  // Type b = T.tapply(T.tcon((Tycon){"Maybe"}), a);
  // TI_RUN(typeOf0(as, e->rhs, a));
  // TI_RUN(typeOf0(as, e->lhs, T.func(a, b)));
  // TI_RUN(unify(b, t), x);
  // TI_RETURN(x);
  // ~~~
}

fn(typeOfFail, TAList, Expr, Type, UnTypeInferArgs(None)) {
  g_bind((as, e, t, s, ok, err), *args);
  TI_FAIL((TypeError){"Invalid expr"});
}

static TypeInfer(None) typeOf0Impl(TAList as, Expr e, Type t) {
  switch (e->id) {
  case VAR: {
    __auto_type f = typeOfVar();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case LAMBDA: {
    __auto_type f = typeOfLambda();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case APPLY: {
    __auto_type f = typeOfApply();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case IFELSE: {
    __auto_type f = typeOfIfelse();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case BLK: {
    __auto_type f = typeOfBlk();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case SEQ: {
    __auto_type f = typeOfSeq();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case DECLVAR: {
    __auto_type f = typeOfDeclvar();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case ASSIGN: {
    __auto_type f = typeOfAssign();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case LET: {
    __auto_type f = typeOfLet();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case OR:
  case AND: {
    __auto_type f = typeOfOrAnd();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case EQ:
  case NEQ:
  case LE:
  case LT:
  case GT:
  case GE: {
    __auto_type f = typeOfComparisson();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case ADD:
  case SUB:
  case MUL:
  case DIV:
  case MOD: {
    __auto_type f = typeOfArithmetic();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case NEG: {
    __auto_type f = typeOfNeg();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case NOT: {
    __auto_type f = typeOfNot();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case NUM: {
    __auto_type f = typeOfNum();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case FALSE:
  case TRUE: {
    __auto_type f = typeOfFalseTrue();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case UNIT: {
    __auto_type f = typeOfUnit();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case PRINT: {
    __auto_type f = typeOfPrint();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case CON: {
    __auto_type f = typeOfCon();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  case CAPPLY: {
    __auto_type f = typeOfCapply();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  default: {
    __auto_type f = typeOfFail();
    return (TypeInfer(None)){fn_apply(f, as, e, t)};
  }
  }
}

static TypeInfer(None) typeOf0(TAList as, Expr e, Type t) {
  return ti_label(typeOf0Impl(as, e, t), e);
}

// -----------------------------------------------------------------------
typedef_Fn_r(TAList, Expr, UnTypeInfer(Type));
fn(typeOfImpl, TAList, Expr, UnTypeInferArgs(Type)) {
  g_bind((as, e, s, ok, err), *args);
  TI_RUN(newTVar(), a);
  TI_RUN(typeOf0(as, e, a));
  TI_RUN(getSubst(), sub);
  TI_RETURN(t_apply_subst(sub, a));
}

TypeInfer(Type) typeOf(TAList as, Expr e) {
  __auto_type f = typeOfImpl();
  return (TypeInfer(Type)){fn_apply(f, as, e)};
}
