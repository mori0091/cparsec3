/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Expr.h"
#include "Scheme.h"
#include "Types.h"

typedef struct TypeAssumption {
  Var var;
  Scheme scheme;
} TypeAssumption;

trait_List(TypeAssumption);
trait_Types(TypeAssumption);
trait_Types(List(TypeAssumption));

typedef List(TypeAssumption) TAList;

trait_Maybe(Scheme);

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
  Scheme (*scheme)(TAList as, Type t);
  /**
   * Finds a scheme bounded to a variable `var` within the list `as`.
   */
  Maybe(Scheme) (*lookup)(Var var, TAList as);
  /**
   * Adds new (var, scheme) assumption to the list `as`.
   */
  TAList (*add)(Var var, Scheme sc, TAList as);
} Assumption;

Assumption Trait(Assumption);
