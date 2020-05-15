/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Subst.h"
#include "Pred.h"

typedef struct Unify {
  /** try to find the most general unifier (MGU) of t1 and t2 */
  Maybe(Subst) (*unifier)(Type t1, Type t2);
  /** try to bind the type to the type-variable */
  Maybe(Subst) (*tbind)(Tyvar tvar, Type t);
  /** try to calculate a matching substitutions */
  Maybe(Subst) (*match)(Type t1, Type t2);
  // ----
  /** try to find the most general unifier (MGU) of p1 and p2 */
  Maybe(Subst) (*mguPred)(Pred p1, Pred p2);
  /** try to calculate a matching substitutions of predicates */
  Maybe(Subst) (*matchPred)(Pred p1, Pred p2);
} Unify;

Unify Trait(Unify);
