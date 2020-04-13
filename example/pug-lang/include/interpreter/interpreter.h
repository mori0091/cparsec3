/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../types/Expr.h"

#include "context.h"

#define Interpreter(...) TYPE_NAME(Interpreter, __VA_ARGS__)

typedef struct RuntimeError {
  String msg;
} RuntimeError;

// typedef_Result(int, RuntimeError);
// typedef Result(int, RuntimeError) EvalResult;
typedef struct EvalResult {
  bool success;
  union {
    int ok;
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

#define EVAL_AS_LVALUE(_ctx_, _x_, _var_)                                \
  EvalResult _var_ = eval_as_lvalue(_ctx_, _x_);                         \
  if (!_var_.success) {                                                  \
    return _var_;                                                        \
  }

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

#define INFIX_OP(_ctx_, _op_, _a_, _b_)                                  \
  do {                                                                   \
    REQUIRE_TYPE_EQ(_a_, _b_);                                           \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    RETURN_OK(lhs.ok _op_ rhs.ok);                                       \
  } while (0)

#define DIV_MOD_OP(_ctx_, _op_, _a_, _b_)                                \
  do {                                                                   \
    REQUIRE_TYPE_EQ(_a_, _b_);                                           \
    EVAL(_ctx_, _a_, lhs);                                               \
    EVAL(_ctx_, _b_, rhs);                                               \
    if (rhs.ok == 0) {                                                   \
      RETURN_ERR("Division by zero");                                    \
    }                                                                    \
    RETURN_OK(lhs.ok _op_ rhs.ok);                                       \
  } while (0)

#define PREFIX_OP(_ctx_, _op_, _b_)                                      \
  do {                                                                   \
    EVAL(_ctx_, _b_, rhs);                                               \
    RETURN_OK(_op_ rhs.ok);                                              \
  } while (0)

#define REQUIRE_TYPE_EQ(lhs, rhs)                                        \
  do {                                                                   \
    if (trait(Eq(Type)).neq(lhs->type, rhs->type)) {                     \
      RETURN_ERR("Type mismatch");                                       \
    }                                                                    \
  } while (0)

static EvalResult eval_as_lvalue(Context* ctx, Expr x) {
  assert(x->kind == VAR);
  ContextT C = trait(Context);
  Maybe(Address) ma = C.map.lookup(ctx, x->var.ident);
  if (ma.none) {
    ma = C.map.put(ctx, x->var.ident);
    if (ma.none) {
      RETURN_ERR("Stack overflow");
    }
  }
  RETURN_OK(ma.value);
}

static EvalResult FUNC_NAME(eval, Interpreter(Expr))(Context* ctx,
                                                     Expr x) {
  switch (x->kind) {
  case SEQ: {
    EVAL(ctx, x->lhs, lhs);
    EVAL(ctx, x->rhs, rhs);
    RETURN_OK(rhs.ok);
  }
  case ASSIGN: {
    REQUIRE_TYPE_EQ(x->lhs, x->rhs);
    EVAL(ctx, x->rhs, rhs);
    EVAL_AS_LVALUE(ctx, x->lhs, lval);
    ContextT C = trait(Context);
    C.stack.store(ctx, lval.ok, rhs.ok);
    RETURN_OK(rhs.ok);
  }
  case EQ:
    INFIX_OP(ctx, ==, x->lhs, x->rhs);
  case NEQ:
    INFIX_OP(ctx, !=, x->lhs, x->rhs);
  case LE:
    INFIX_OP(ctx, <=, x->lhs, x->rhs);
  case LT:
    INFIX_OP(ctx, <, x->lhs, x->rhs);
  case GT:
    INFIX_OP(ctx, >, x->lhs, x->rhs);
  case GE:
    INFIX_OP(ctx, >=, x->lhs, x->rhs);
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
  case NEG:
    PREFIX_OP(ctx, -, x->rhs);
  case NOT:
    PREFIX_OP(ctx, !, x->rhs);
  case NUM:
    RETURN_OK(x->num.value);
  case VAR: {
    EVAL_AS_LVALUE(ctx, x, lval);
    ContextT C = trait(Context);
    RETURN_OK(C.stack.load(ctx, lval.ok).value);
  }
  case UNIT:
    RETURN_OK(0);
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
