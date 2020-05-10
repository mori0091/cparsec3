/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Expr.h"
#include "TypeScheme.h"
#include "TypeVarProc.h"

typedef struct TypeAssumption {
  Var var;
  TypeScheme scheme;
} TypeAssumption;

trait_List(TypeAssumption);
trait_TypeVarProc(TypeAssumption);
trait_TypeVarProc(List(TypeAssumption));

trait_Maybe(TypeScheme);

typedef struct Assumption {
  /**
   * Creates a type scheme that all type variables of `t` but not included in
   * assumption `as` was replaced with universal quantified type variables.
   */
  TypeScheme (*scheme)(List(TypeAssumption) as, Type t);
  /**
   * Finds a scheme bounded to a variable `var` within assumption `as`.
   */
  Maybe(TypeScheme) (*lookup)(Var var, List(TypeAssumption) as);
  /**
   * Adds new (var, scheme) assumption to the list.
   */
  List(TypeAssumption) (*add)(Var var, TypeScheme sc, List(TypeAssumption) as);
} Assumption;

Assumption Trait(Assumption);
