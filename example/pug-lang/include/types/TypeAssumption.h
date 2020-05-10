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

typedef List(TypeAssumption) TAList;

trait_Maybe(TypeScheme);

typedef struct Assumption {
  /**
   * Creates a type scheme that all type variables of `t` but not included
   * in the assumption list `as` was replaced with universal quantified
   * type variables.
   *
   * In other words, replaces all unknown type variables in `t` with
   * wildcards.
   *
   * For example:
   * when `t` was `s -> u1 -> u2`, where u1 and u2 was unknown.
   * then it results a scheme `∀a.∀b.s -> a -> b`.
   */
  TypeScheme (*scheme)(TAList as, Type t);
  /**
   * Finds a scheme bounded to a variable `var` within the list `as`.
   */
  Maybe(TypeScheme) (*lookup)(Var var, TAList as);
  /**
   * Adds new (var, scheme) assumption to the list `as`.
   */
  TAList (*add)(Var var, TypeScheme sc, TAList as);
} Assumption;

Assumption Trait(Assumption);
