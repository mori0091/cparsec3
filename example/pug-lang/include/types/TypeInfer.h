/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include "Assump.h"
#include "Instantiate.h"
#include "Scheme.h"
#include "Subst.h"
#include "Type.h"
#include "TypeSystem.h"

typedef struct TIState {
  /** current type substitution */
  Subst subst;
  /** next index for creating universal quantified type variable */
  int i;
} TIState;

typedef struct TypeError {
  String msg;
} TypeError;

#define A_STATE_TYPE TIState
#define A_ERROR_TYPE TypeError
#define A_RETURN_TYPES Type, Subst, Scheme, Qual(Type)

#include "../monad/Action.h"

// TypeInfer (type inference monad)
#define TypeInfer(T) ACTION(T)
#define TIResult(T) A_RESULT(T)

#define runTypeInferP(ti, st) runAction(ti, st)
#define runTypeInfer(ti) runAction(ti, ((TIState){0}))

/**
 * Creates TypeInfer monad that gets current type-substitution.
 */
TypeInfer(Subst) getSubst(void);

/**
 * Creates TypeInfer monad that extends current type-substitution.
 */
TypeInfer(None) extSubst(Subst sub);

/**
 * Creates TypeInfer monad that extends current type-substitution by the
 * most general unifier of t1 and t2.
 */
TypeInfer(None) unify(Type t1, Type t2);

/**
 * Creates TypeInfer monad that creates new type-variable.
 */
TypeInfer(Type) newTVar(Kind k);

/**
 * Creates TypeInfer monad that creates new type from type-scheme.
 *
 * Assuming that the type-scheme was `∀a∀b.(a,b,b,c)`, resulting new type
 * will be for example `(t1,t2,t2,c)`; where `t1` and `t2` are new unique
 * type variables.
 */
TypeInfer(Qual(Type)) freshInst(Scheme sc);

/**
 * Creates TypeInfer monad that infers type of expression `e`.
 */
TypeInfer(Type) typeOf(List(Assump) as, Expr e);

static inline TIResult(Type) testInferP(List(Assump) as, Expr e) {
  return runTypeInfer(typeOf(as, e));
}

static inline TIResult(Type) testInfer(Expr e) {
  return testInferP(NULL, e);
}
