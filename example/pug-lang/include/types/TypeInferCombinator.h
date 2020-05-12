/* -*- coding: utf-8-unix -*- */
#pragma once

#include "types/TypeSystem.h"

/**
 * Creates TypeInfer monad that gets current type-substitution.
 */
TypeInfer(Subst) getSubst(void);

/**
 * Creates TypeInfer monad that extends current type-substitution by the
 * most general unifier of t1 and t2.
 */
TypeInfer(None) unify(Type t1, Type t2);

/**
 * Creates TypeInfer monad that creates new type-variable.
 */
TypeInfer(Type) newTVar(void);

/**
 * Creates TypeInfer monad that creates new type from type-scheme.
 *
 * Assuming that the type-scheme was `∀a∀b.(a,b,b,c)`, resulting new type
 * will be for example `(t1,t2,t2,c)`; where `t1` and `t2` are new unique
 * type variables.
 */
TypeInfer(Type) freshInst(Scheme sc);

/**
 * Creates TypeInfer monad that infers type of expression `e`.
 */
TypeInfer(Type) typeOf(TAList as, Expr e);

static inline TIResult(Type) testInferP(TAList as, Expr e) {
  return runTypeInfer(typeOf(as, e));
}

static inline TIResult(Type) testInfer(Expr e) {
  return testInferP(NULL, e);
}
