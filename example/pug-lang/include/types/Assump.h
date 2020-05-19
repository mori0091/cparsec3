/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Expr.h"
#include "Scheme.h"
#include "Types.h"
#include "Qual.h"

typedef struct Assump {
  Id ident;
  Scheme scheme;
} Assump;

trait_Eq(Assump);

trait_List(Assump);
trait_Eq(List(Assump));

trait_Types(Assump);
trait_Types(List(Assump));

trait_Qual(Assump);
trait_Qual(List(Assump));
trait_Types(Qual(Assump));
trait_Types(Qual(List(Assump)));

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
  Scheme (*scheme)(List(Assump) as, Type t);
  /**
   * Finds a scheme bounded to an identifier `ident` within the list `as`.
   */
  Maybe(Scheme) (*lookup)(Id ident, List(Assump) as);
  /**
   * Adds new (var, scheme) assumption to the list `as`.
   */
  List(Assump) (*add)(Id ident, Scheme sc, List(Assump) as);
} Assumption;

Assumption Trait(Assumption);
