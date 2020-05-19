/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ClassEnv.h"
#include "TypeInfer.h"

// -----------------------------------------------------------------------
// middle level type-inference monads
// -----------------------------------------------------------------------

/**
 * Creates type-inference monad that infers type of literal `lit`.
 */
TI(Tup(List(Pred), Type)) tiLiteral(Literal lit);

/**
 * Creates type-inference monad that infers type of expression `e`.
 */
TI(Tup(List(Pred), Type)) tiExpr(List(Assump) as, Expr e);

// -----------------------------------------------------------------------
// high level type-inference monads
// -----------------------------------------------------------------------

/**
 * Creates type-inference monad that infers type of program `e`.
 *
 * NOTE: Only `tiProgram` can provide final judgement of type-inference.
 * Any other type-inference monads are part of `tiProgram` and they
 * provide a partial judgement. ("partial" means that it needs to apply
 * substitution just once more.)
 */
TI(Tup(List(Pred), Type)) tiProgram(ClassEnv ce, List(Assump) as, Expr e);

// -----------------------------------------------------------------------
static inline TIResult(Tup(List(Pred), Type))
    testInferP(ClassEnv ce, List(Assump) as, Expr e) {
  return runTI(tiProgram(ce, as, e), ((TIState){0}));
}

static inline TIResult(Tup(List(Pred), Type)) testInfer(Expr e) {
  ClassEnv ce = initialEnv();
  return testInferP(ce, NULL, e);
}
