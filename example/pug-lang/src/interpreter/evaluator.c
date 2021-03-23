/* -*- coding: utf-8-unix -*- */

#include "interpreter/evaluator.h"

#include "interpreter/interpreter.h"

Result(Expr, String) eval(Expr e) {
  Context ctx = trait(Context).create();
  EvalResult r = trait(Interpreter(Expr)).eval(ctx, e);
  if (!r.success) {
    return (Result(Expr, String)){
        .err = r.err.msg,
    };
  }
  return (Result(Expr, String)){
      .success = true,
      .ok = r.ok,
  };
}
