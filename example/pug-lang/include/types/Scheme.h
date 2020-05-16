/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Qual.h"
#include "Subst.h"
#include "Type.h"
#include "Types.h"

typedef struct Scheme Scheme;
struct Scheme {
  List(Kind) ks;
  Qual(Type) qt;
};

Scheme Forall(List(Kind) ks, Qual(Type) qt);
Scheme quantify(List(Tyvar) vs, Qual(Type) qt);
Scheme toScheme(Type t);

List(Kind) mapKind(List(Tyvar) vs); /* map kind vs */
Subst toSubst(List(Tyvar) vs);      /* zip vs (map TGen [0..]) */

trait_Eq(Scheme);

trait_List(Scheme);
trait_Eq(List(Scheme));

trait_Types(Scheme);
trait_Types(List(Scheme));

trait_Qual(Scheme);
trait_Qual(List(Scheme));
trait_Types(Qual(Scheme));
trait_Types(Qual(List(Scheme)));
