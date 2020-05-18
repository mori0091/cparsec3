/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ClassEnv.h"
#include "TypeInfer.h"

// -----------------------------------------------------------------------
// high level type-inference monads
// -----------------------------------------------------------------------

/**
 * Creates type-inference monad that infers type of expression `e`.
 */
TI(Infered(Type)) tiExpr(List(Assump) as, Expr e);

/**
 * Creates type-inference monad that infers type of program `e`.
 *
 * NOTE: Only `tiProgram` can provide final judgement of type-inference.
 * Any other type-inference monads are part of `tiProgram` and they
 * provide a partial judgement. ("partial" means that it needs to apply
 * substitution just once more.)
 */
TI(Infered(Type)) tiProgram(ClassEnv ce, List(Assump) as, Expr e);

// -----------------------------------------------------------------------
static inline TIResult(Infered(Type))
    testInferP(ClassEnv ce, List(Assump) as, Expr e) {
  return runTI(tiProgram(ce, as, e), ((TIState){0}));
}

static inline TIResult(Infered(Type)) testInfer(Expr e) {
  ClassEnv ce = initialEnv();
  return testInferP(ce, NULL, e);
}
