/* -*- coding: utf-8-unix -*- */

#include "interpreter/interpreter.h"

// -----------------------------------------------------------------------
static EvalResult eval_expr1(Context ctx, Expr x);
static EvalResult eval_expr(Context ctx, Expr x);

Interpreter(Expr) Trait(Interpreter(Expr)) {
  return (Interpreter(Expr)){
      .eval = eval_expr,
  };
}

// -----------------------------------------------------------------------
#define EVAL(_ctx_, _x_, _var_)                                          \
  EvalResult _var_ = eval_expr(_ctx_, _x_);                              \
  if (!_var_.success) {                                                  \
    return _var_;                                                        \
  }

#define RETURN_OK(_x_) return EVAL_RESULT_OK(_x_)
#define RETURN_DEFERED(_ctx_, _x_) return EVAL_RESULT_DEFERED(_ctx_, _x_)
#define RETURN_ERR(_msg_) return EVAL_RESULT_ERR(_msg_)

#define INFIX_COMPARISON_OP(_ctx_, _op_, _a_, _b_)                       \
  do {                                                                   \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    REQUIRE_TYPE_EQ(lhs.ok->type, rhs.ok->type);                         \
    switch (lhs.ok->kind) {                                              \
    case NUM: {                                                          \
      bool x = lhs.ok->num.value _op_ rhs.ok->num.value;                 \
      RETURN_OK(trait(Expr).boolean(x));                                 \
    }                                                                    \
    case FALSE:                                                          \
    case TRUE:                                                           \
    case UNIT: {                                                         \
      bool x = lhs.ok->kind _op_ rhs.ok->kind;                           \
      RETURN_OK(trait(Expr).boolean(x));                                 \
    }                                                                    \
    case CLOSURE: {                                                      \
      bool x = lhs.ok _op_ rhs.ok;                                       \
      RETURN_OK(trait(Expr).boolean(x));                                 \
    }                                                                    \
    default:                                                             \
      RETURN_ERR("Type error");                                          \
    }                                                                    \
  } while (0)

#define INFIX_OP(_ctx_, _op_, _a_, _b_)                                  \
  do {                                                                   \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    REQUIRE_TYPE_EQ(lhs.ok->type, TYPE(int));                            \
    REQUIRE_TYPE_EQ(rhs.ok->type, TYPE(int));                            \
    int64_t x = lhs.ok->num.value _op_ rhs.ok->num.value;                \
    RETURN_OK(trait(Expr).num((Num){x}));                                \
  } while (0)

#define DIV_MOD_OP(_ctx_, _op_, _a_, _b_)                                \
  do {                                                                   \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    REQUIRE_TYPE_EQ(lhs.ok->type, TYPE(int));                            \
    REQUIRE_TYPE_EQ(rhs.ok->type, TYPE(int));                            \
    if (rhs.ok->num.value == 0) {                                        \
      RETURN_ERR("Division by zero");                                    \
    }                                                                    \
    int64_t x = lhs.ok->num.value _op_ rhs.ok->num.value;                \
    RETURN_OK(trait(Expr).num((Num){x}));                                \
  } while (0)

#define REQUIRE_TYPE_EQ(lhs, rhs)                                        \
  do {                                                                   \
    if (trait(Eq(Type)).neq(lhs, rhs)) {                                 \
      RETURN_ERR("Type mismatch");                                       \
    }                                                                    \
  } while (0)

// -----------------------------------------------------------------------
static EvalResult eval_apply(Context ctx, Expr x) {
  EVAL(ctx, x->lhs, f);
  if (f.ok->kind != CLOSURE) {
    RETURN_ERR("function application");
  }
  /* TODO: what should be done to do lazy evaluation? */
  EVAL(ctx, x->rhs, a);          // <-
  Expr v = f.ok->lambda->lhs;    // (Var v)
  Expr body = f.ok->lambda->rhs; // body
  ContextT C = trait(Context);
  Context c = C.branch(f.ok->ctx);
  C.map.put(c, v->var.ident, a.ok);
  RETURN_DEFERED(c, body);
}

static EvalResult eval_lambda(Context ctx, Expr x) {
  ContextT C = trait(Context);
  ExprT E = trait(Expr);
  RETURN_OK(E.closure(C.nested(ctx), x));
}

static EvalResult eval_ifelse(Context ctx, Expr x) {
  ContextT C = trait(Context);
  // make implicit block scope that encloses whole if~else block
  Context c = C.branch(ctx);
  EVAL(c, x->lhs, cond);
  REQUIRE_TYPE_EQ(cond.ok->type, TYPE(bool));
  Expr then_blk = x->rhs->lhs;
  Expr else_blk = x->rhs->rhs;
  // REQUIRE_TYPE_EQ(then_blk->type, else_blk->type);
  RETURN_DEFERED(c, (cond.ok->kind == TRUE ? then_blk : else_blk));
}

static EvalResult eval_block(Context ctx, Expr x) {
  ContextT C = trait(Context);
  RETURN_DEFERED(C.branch(ctx), x->rhs);
}

static EvalResult eval_seq(Context ctx, Expr x) {
  EVAL(ctx, x->lhs, lhs);
  RETURN_DEFERED(ctx, x->rhs);
}

static EvalResult eval_let(Context ctx, Expr x) {
  ContextT C = trait(Context);
  assert(x->lhs->kind == VAR);
  // if the previous definiton exists, it will be shadowed.
  C.map.put(ctx, x->lhs->var.ident, x->rhs);
  RETURN_OK(x->rhs);
}

static EvalResult eval_assign(Context ctx, Expr x) {
  ContextT C = trait(Context);
  assert(x->lhs->kind == VAR);
  EVAL(ctx, x->rhs, rhs);
  EVAL(ctx, x->lhs, lhs);
  // types must be same with previous definition
  REQUIRE_TYPE_EQ(lhs.ok->type, rhs.ok->type);
  // the previous definiton will be shadowed.
  C.map.put(ctx, x->lhs->var.ident, rhs.ok);
  RETURN_OK(rhs.ok);
}

static EvalResult eval_logical_AND_OR(Context ctx, Expr x) {
  EVAL(ctx, x->lhs, lhs);
  REQUIRE_TYPE_EQ(lhs.ok->type, TYPE(bool));
  if (lhs.ok->kind == (x->kind == OR ? TRUE : FALSE)) {
    RETURN_OK(lhs.ok);
  }
  EVAL(ctx, x->rhs, rhs);
  REQUIRE_TYPE_EQ(rhs.ok->type, TYPE(bool));
  RETURN_OK(rhs.ok);
}

static EvalResult eval_print(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  EVAL(ctx, x->rhs, rhs);
  printf("%s\n", trait(Show(Expr)).show(rhs.ok));
  RETURN_OK(E.unit());
}

static EvalResult eval_negate(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  Expr y = E.sub(E.num((Num){0}), x->rhs);
  EVAL(ctx, y, rhs);
  RETURN_OK(rhs.ok);
}

static EvalResult eval_not(Context ctx, Expr x) {
  ExprT E = trait(Expr);
  EVAL(ctx, x->rhs, rhs);
  switch (rhs.ok->kind) {
  case NUM:
    RETURN_OK(E.num((Num){~rhs.ok->num.value}));
  case TRUE:
    RETURN_OK(E.boolean(false));
  case FALSE:
    RETURN_OK(E.boolean(true));
  default:
    RETURN_ERR("Type error");
  }
}

static EvalResult eval_var(Context ctx, Expr x) {
  ContextT C = trait(Context);
  MapEntry* m = C.map.lookup(ctx, x->var.ident);
  if (!m) {
    RETURN_ERR("Undefined variable");
  }
  EVAL(ctx, m->e, val);
  m->e = val.ok; /* replace with evaluated value */
  RETURN_OK(val.ok);
}

// -----------------------------------------------------------------------
static EvalResult eval_expr1(Context ctx, Expr x) {
  switch (x->kind) {
  case APPLY:
    return eval_apply(ctx, x);
  case LAMBDA:
    return eval_lambda(ctx, x);
  case IFELSE:
    return eval_ifelse(ctx, x);
  case BLK:
    return eval_block(ctx, x);
  case SEQ:
    return eval_seq(ctx, x);
  case LET:
    return eval_let(ctx, x);
  case ASSIGN:
    return eval_assign(ctx, x);
  case OR:
  case AND:
    return eval_logical_AND_OR(ctx, x);
  case EQ:
    INFIX_COMPARISON_OP(ctx, ==, x->lhs, x->rhs);
  case NEQ:
    INFIX_COMPARISON_OP(ctx, !=, x->lhs, x->rhs);
  case LE:
    INFIX_COMPARISON_OP(ctx, <=, x->lhs, x->rhs);
  case LT:
    INFIX_COMPARISON_OP(ctx, <, x->lhs, x->rhs);
  case GT:
    INFIX_COMPARISON_OP(ctx, >, x->lhs, x->rhs);
  case GE:
    INFIX_COMPARISON_OP(ctx, >=, x->lhs, x->rhs);
  case ADD:
    INFIX_OP(ctx, +, x->lhs, x->rhs);
  case SUB:
    INFIX_OP(ctx, -, x->lhs, x->rhs);
  case MUL:
    INFIX_OP(ctx, *, x->lhs, x->rhs);
  case DIV:
    DIV_MOD_OP(ctx, /, x->lhs, x->rhs);
  case MOD:
    DIV_MOD_OP(ctx, %, x->lhs, x->rhs);
  case PRINT:
    return eval_print(ctx, x);
  case NEG:
    return eval_negate(ctx, x);
  case NOT:
    return eval_not(ctx, x);
  case VAR:
    return eval_var(ctx, x);
  case NUM:
  case TRUE:
  case FALSE:
  case UNIT:
  case CLOSURE:
    RETURN_OK(x);
  default:
    RETURN_ERR("Illegal Expr");
  }
}

static inline bool is_defered(EvalResult r) {
  return r.ctx != 0;
}

static EvalResult eval_expr(Context ctx, Expr x) {
  EvalResult r = eval_expr1(ctx, x);
  while (is_defered(r)) {
    r = eval_expr1(r.ctx, r.ok);
  }
  return r;
}