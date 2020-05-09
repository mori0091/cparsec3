/* -*- coding: utf-8-unix -*- */
#pragma once

#include "TypeSubst.h"

trait_Maybe(TypeSubst);

typedef struct TypeUnify {
  /** try to find the most general unifier (MGU) of t1 and t2 */
  Maybe(TypeSubst) (*unifier)(Type t1, Type t2);
  /** try to bind the type to the type-variable */
  Maybe(TypeSubst) (*tbind)(TVar tvar, Type t);
} TypeUnify;

TypeUnify Trait(TypeUnify);
