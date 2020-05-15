/* -*- coding: utf-8-unix -*- */
#pragma once

#include "Subst.h"
#include "Type.h"
#include "Types.h"

typedef struct Scheme {
  int numTGen; ///< number of `TGen` in the `type`.
  Type type;   ///< type prototype.
} Scheme;

trait_Eq(Scheme);

trait_List(Scheme);
trait_Eq(List(Scheme));

trait_Types(Scheme);
trait_Types(List(Scheme));
