/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Subst.h"

typedef struct Unify {
  /** try to find the most general unifier (MGU) of t1 and t2 */
  Maybe(Subst) (*unifier)(Type t1, Type t2);
  /** try to bind the type to the type-variable */
  Maybe(Subst) (*tbind)(Tyvar tvar, Type t);
} Unify;

Unify Trait(Unify);
