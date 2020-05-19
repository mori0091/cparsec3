/* -*- coding: utf-8-unix -*- */
#pragma once

#include <cparsec3/base/base_generics.h>

#include "Assump.h"
#include "Instantiate.h"
#include "Pat.h"
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

#define Tup(...) TYPE_NAME(Tup, __VA_ARGS__)

typedef struct Tup(List(Pred), Type) {
  List(Pred) ps;
  Type t;
}
Tup(List(Pred), Type);

typedef struct Tup(List(Pred), List(Assump), Type) {
  List(Pred) ps;
  List(Assump) as;
  Type t;
}
Tup(List(Pred), List(Assump), Type);

typedef struct Tup(List(Pred), List(Assump), List(Type)) {
  List(Pred) ps;
  List(Assump) as;
  List(Type) ts;
}
Tup(List(Pred), List(Assump), List(Type));

#define A_STATE_TYPE TIState
#define A_ERROR_TYPE TypeError
#define A_RETURN_TYPES                                                   \
  Type, Subst, Scheme, Qual(Type), Tup(List(Pred), Type),                \
      Tup(List(Pred), List(Assump), Type),                               \
      Tup(List(Pred), List(Assump), List(Type))

#include "../monad/Action.h"

/** type-inference monad */
#define TI(T) ACTION(T)

/** result type of `runTI()` */
#define TIResult(T) A_RESULT(T)

/**
 * generic macro to run type-inference monad `ti` with initial state `st`.
 * \param ti   a type-inference monad of type `TI(T)`
 * \param st   initial state of type `TIState`
 */
#define runTI(ti, st) runAction(ti, st)

// -----------------------------------------------------------------------
// low level type-inference monads
// -----------------------------------------------------------------------

/**
 * Creates type-inference monad that gets current type-substitution.
 */
TI(Subst) getSubst(void);

/**
 * Creates type-inference monad that extends current type-substitution.
 */
TI(None) extSubst(Subst sub);

/**
 * Creates type-inference monad that extends current type-substitution by
 * the most general unifier of t1 and t2.
 */
TI(None) unify(Type t1, Type t2);

/**
 * Creates type-inference monad that creates new type-variable.
 */
TI(Type) newTVar(Kind k);

/**
 * Creates type-inference monad that creates new type from type-scheme.
 *
 * Assuming that the type-scheme was `∀a∀b.(a,b,b,c)`, resulting new type
 * will be for example `(t1,t2,t2,c)`; where `t1` and `t2` are new unique
 * type variables.
 */
TI(Qual(Type)) freshInst(Scheme sc);
