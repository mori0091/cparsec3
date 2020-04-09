/* -*- coding: utf-8-unix -*- */
#pragma once

#include "../types/Expr.h"

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
  EvalResult (*eval)(Expr x);
};

Interpreter(Expr) Trait(Interpreter(Expr));

// -----------------------------------------------------------------------
#if defined(CPARSEC_CONFIG_IMPLEMENT)

#define EVAL(_x_, _var_)                                                 \
  EvalResult _var_ = FUNC_NAME(eval, Interpreter(Expr))(_x_);            \
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

#define INFIX_OP(_op_, _a_, _b_)                                         \
  do {                                                                   \
    EVAL(_a_, lhs);                                                      \
    EVAL(_b_, rhs);                                                      \
    RETURN_OK(lhs.ok _op_ rhs.ok);                                       \
  } while (0)

#define DIV_MOD_OP(_op_, _a_, _b_)                                       \
  do {                                                                   \
    EVAL(_a_, lhs);                                                      \
    EVAL(_b_, rhs);                                                      \
    if (rhs.ok == 0) {                                                   \
      RETURN_ERR("Division by zero");                                    \
    }                                                                    \
    RETURN_OK(lhs.ok _op_ rhs.ok);                                       \
  } while (0)

#define PREFIX_OP(_op_, _b_)                                             \
  do {                                                                   \
    EVAL(_b_, rhs);                                                      \
    RETURN_OK(_op_ rhs.ok);                                              \
  } while (0)

static EvalResult FUNC_NAME(eval, Interpreter(Expr))(Expr x) {
  switch (x->type) {
  case EQ:
    INFIX_OP(==, x->lhs, x->rhs);
  case NEQ:
    INFIX_OP(!=, x->lhs, x->rhs);
  case LE:
    INFIX_OP(<=, x->lhs, x->rhs);
  case LT:
    INFIX_OP(<, x->lhs, x->rhs);
  case GT:
    INFIX_OP(>, x->lhs, x->rhs);
  case GE:
    INFIX_OP(>=, x->lhs, x->rhs);
  case ADD:
    INFIX_OP(+, x->lhs, x->rhs);
  case SUB:
    INFIX_OP(-, x->lhs, x->rhs);
  case MUL:
    INFIX_OP(*, x->lhs, x->rhs);
  case DIV:
    DIV_MOD_OP(/, x->lhs, x->rhs);
  case MOD:
    DIV_MOD_OP(%, x->lhs, x->rhs);
  case NEG:
    PREFIX_OP(-, x->rhs);
  case NOT:
    PREFIX_OP(!, x->rhs);
  case NUM:
    RETURN_OK(x->num.value);
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
