/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../types/Expr.h"

#include "context.h"

#define Interpreter(...) TYPE_NAME(Interpreter, __VA_ARGS__)

typedef struct RuntimeError {
  String msg;
} RuntimeError;

// typedef_Result(Expr, RuntimeError);
// typedef Result(Expr, RuntimeError) EvalResult;
typedef struct EvalResult {
  bool success;
  union {
    Expr ok;
    RuntimeError err;
  };
} EvalResult;

typedef struct Interpreter(Expr) Interpreter(Expr);
struct Interpreter(Expr) {
  EvalResult (*eval)(Context * ctx, Expr x);
};

Interpreter(Expr) Trait(Interpreter(Expr));

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

#define EVAL(_ctx_, _x_, _var_)                                          \
  EvalResult _var_ = FUNC_NAME(eval, Interpreter(Expr))(_ctx_, _x_);     \
  if (!_var_.success) {                                                  \
    return _var_;                                                        \
  }

#define RETURN_OK(_x_)                                                   \
  return (EvalResult) {                                                  \
    .success = true, .ok = _x_                                           \
  }

#define RETURN_ERR(_msg_)                                                \
  return (EvalResult) {                                                  \
    .err.msg = _msg_                                                     \
  }

#define INFIX_BOOL_OP(_ctx_, _op_, _a_, _b_)                             \
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
    int x = lhs.ok->num.value _op_ rhs.ok->num.value;                    \
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
    int x = lhs.ok->num.value _op_ rhs.ok->num.value;                    \
    RETURN_OK(trait(Expr).num((Num){x}));                                \
  } while (0)

#define REQUIRE_TYPE_EQ(lhs, rhs)                                        \
  do {                                                                   \
    if (trait(Eq(Type)).neq(lhs, rhs)) {                                 \
      RETURN_ERR("Type mismatch");                                       \
    }                                                                    \
  } while (0)

static EvalResult FUNC_NAME(eval, Interpreter(Expr))(Context* ctx,
                                                     Expr x) {
  ContextT C = trait(Context);
  ExprT E = trait(Expr);
  switch (x->kind) {
  case BLK: {
    EVAL(C.branch(ctx), x->rhs, rhs);
    RETURN_OK(rhs.ok);
  }
  case SEQ: {
    EVAL(ctx, x->lhs, lhs);
    EVAL(ctx, x->rhs, rhs);
    RETURN_OK(rhs.ok);
  }
  case LET: {
    assert(x->lhs->kind == VAR);
    EVAL(ctx, x->rhs, rhs);
    // if the previous definiton exists, it will be shadowed.
    C.map.put(ctx, x->lhs->var.ident, rhs.ok);
    RETURN_OK(rhs.ok);
  }
  case ASSIGN: {
    assert(x->lhs->kind == VAR);
    EVAL(ctx, x->rhs, rhs);
    EVAL(ctx, x->lhs, lhs);
    // types must be same with previous definition
    REQUIRE_TYPE_EQ(lhs.ok->type, rhs.ok->type);
    // the previous definiton will be shadowed.
    C.map.put(ctx, x->lhs->var.ident, rhs.ok);
    RETURN_OK(rhs.ok);
  }
  case EQ:
    INFIX_BOOL_OP(ctx, ==, x->lhs, x->rhs);
  case NEQ:
    INFIX_BOOL_OP(ctx, !=, x->lhs, x->rhs);
  case LE:
    INFIX_BOOL_OP(ctx, <=, x->lhs, x->rhs);
  case LT:
    INFIX_BOOL_OP(ctx, <, x->lhs, x->rhs);
  case GT:
    INFIX_BOOL_OP(ctx, >, x->lhs, x->rhs);
  case GE:
    INFIX_BOOL_OP(ctx, >=, x->lhs, x->rhs);
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
  case NEG: {
    Expr y = E.sub(E.num((Num){0}), x->rhs);
    EVAL(ctx, y, rhs);
    RETURN_OK(rhs.ok);
  }
  case NOT: {
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
  case VAR: {
    ContextT C = trait(Context);
    Maybe(Expr) ma = C.map.lookup(ctx, x->var.ident);
    if (ma.none) {
      RETURN_ERR("Undefined variable");
    }
    RETURN_OK(ma.value);
  }
  case NUM:
  case TRUE:
  case FALSE:
  case UNIT:
    RETURN_OK(x);
  default:
    RETURN_ERR("Illegal Expr");
  }
}

Interpreter(Expr) Trait(Interpreter(Expr)) {
  return (Interpreter(Expr)){
      .eval = FUNC_NAME(eval, Interpreter(Expr)),
  };
}

#undef EVAL
#undef RETURN_OK
#undef RETURN_ERR
#undef INFIX_OP
#undef DIV_MOD_OP
#undef PREFIX_OP

#endif
