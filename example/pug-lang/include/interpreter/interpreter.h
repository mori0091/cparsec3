/* -*- coding: utf-8-unix -*- */
#pragma once

#include "types/Context.h"
#include "types/Expr.h"

#define Interpreter(...) TYPE_NAME(Interpreter, __VA_ARGS__)

#define EVAL_RESULT_OK(_x_)                                              \
  ((EvalResult){                                                         \
      .success = true,                                                   \
      .ok = _x_,                                                         \
  })

#define EVAL_RESULT_ERR(_msg_)                                           \
  ((EvalResult){                                                         \
      .err.msg = _msg_,                                                  \
  })

typedef struct RuntimeError {
  String msg;
} RuntimeError;

typedef struct EvalResult {
  bool success;
  union {
    Expr ok;
    RuntimeError err;
  };
} EvalResult;

typedef struct Interpreter(Expr) Interpreter(Expr);
struct Interpreter(Expr) {
  EvalResult (*eval)(Context ctx, Expr x);
};

Interpreter(Expr) Trait(Interpreter(Expr));
