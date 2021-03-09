/* -*- coding: utf-8-unix -*- */
#pragma once

#include "ClassEnv.h"
#include "TypeInfer.h"

#include "Expr.h"

// -----------------------------------------------------------------------
// middle level type-inference monads
// -----------------------------------------------------------------------

/**
 * Creates type-inference monad that infers type of literal `lit`.
 */
TI(Tup(List(Pred), Type)) tiLiteral(Literal lit);

/**
 * Creates type-inference monad that infers type of pattern `pat`.
 */
TI(Tup(List(Pred), List(Assump), Type)) tiPat(Pat pat);

/**
 * Creates type-inference monad that infers type of list of pattern
 * `pats`.
 */
TI(Tup(List(Pred), List(Assump), List(Type))) tiPats(List(Pat) pats);

/**
 * Creates type-inference monad that infers type of expression `e`.
 */
TI(Tup(List(Pred), Type)) tiExpr(List(Assump) as, Expr e);

// -----------------------------------------------------------------------
// high level type-inference monads
// -----------------------------------------------------------------------

/**
 * Creates type-inference monad that infers type of alternative `alt`.
 */
TI(Tup(List(Pred), Type)) tiAlt(ClassEnv ce, List(Assump) as, Alt alt);

/**
 * Creates type-inference monad that infers over a list of alternatives
 * `alts` and check that thre returned type in each case agrees with some
 * known type `t`.
 */
TI(List(Pred))
tiAlts(ClassEnv ce, List(Assump) as, List(Alt) alts, Type t);

/**
 * Creates type-inference monad that infers type of program `e`.
 *
 * NOTE: Only `tiProgram` can provide final judgement of type-inference.
 * Any other type-inference monads are part of `tiProgram` and they
 * provide a partial judgement. ("partial" means that it needs to apply
 * substitution just once more.)
 */
TI(Tup(List(Pred), Type)) tiProgram(ClassEnv ce, List(Assump) as, Expr e);
